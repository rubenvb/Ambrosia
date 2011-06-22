/**
  * Parser/nectar_loader.h
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class include
#include "nectar_loader.h"

// libAmbrosia includes
#include "algorithm.h"
#include "ambrosia_config.h"
#include "debug.h"
#include "global.h"
#include "enums.h"
#include "enum_maps.h"
#include "nectar.h"
#include "status.h"
#include "target.h"

// C++ includes
#include <deque>
    using std::deque;
#include <functional>
    using std::function;
    using namespace std::placeholders;
#include <fstream>
    using std::ifstream;
#include <ios>
    using std::noskipws;
#include <istream>
    using std::istream;
    using std::istream_iterator;
#include <iterator>
    using std::back_insert_iterator;
#include <locale>
#include <memory>
    using std::unique_ptr;
/* <set> */
    using std::set;
#include <stack>
    using std::stack;
#include <stdexcept>
    using std::runtime_error;
/* <string> */
    using std::string;
/* <utility> */
    using std::pair;
/* <vector> */
    using std::vector;

libambrosia_namespace_begin

const set<char> s_special_characters = { '(', ')', '{', '}', ':', ',' };
const set<char> s_special_characters_newline = { '(', ')', '{', '}', ':', ',', '\n' };

nectar_loader::nectar_loader( const string &filename, istream &stream,
                              const dependency_list &list )
:   m_filename( filename ),
    m_stream( stream ),
    m_line_number( 1 ),
    m_dependency_list( list ),
    m_global_processed( false ),
    p_target()
{   }
nectar_loader::~nectar_loader()
{
    if( p_target != nullptr )
        emit_error( "Parsing " + p_target->name() + " in file " + m_filename + " failed." );
}

void nectar_loader::extract_nectar( target_list &targets )
{
    debug(2) << "nectar_loader::extract_nectar::Processing file: " << m_filename << ".\n";

    // Remove leading BOM
    skip_BOM( m_stream );

    string token;
    while( next_token(token) )
    {
        debug(3) << "nectar_loader::extract_nectar::checking error_status.\n";
        if( error_status() )
            return;

        debug(3) << "nectar_loader::extract_nectar::processing token: \'" << token << "\'.\n";
        if( "(" == token )
        {
            if( !process_outer_conditional() )
                return; // emit_error already called
        }
        else if( "global" == token )
        {
            debug(4) << "nectar_loader::extract_nectar::global section found at line " << m_line_number << ".\n";
            if( m_global_processed )
                return syntax_error( "Second global section found in nectar file. Only one global section per *.nectar.txt file is allowed." );

            if( next_token(token) && "{" == token )
            {
                p_target = std::unique_ptr<target>( new target("global", target_type::global, dependency_list()) );

                parse_target();
                if( error_status() )
                    return;

                targets.emplace_back( std::move(p_target) );
            }
            else
                return syntax_error( "\'global\' must be followed by \'{\'." );
        }
        else if( "app" == token || "lib" == token )
        {
            const target_type type( (*target_type_map.find(token)).second );
            debug(4) << "nectar_loader::extract_nectar::" << token << " section found at line " << m_line_number << ".\n";
            if( next_token(token) )
            {
                if( "{" == token )
                    return syntax_error( "Expected " + token + " name after " + token + "." );
                else
                {
                    debug(4) << "nectar_loader::extract_nectar::Processing "
                             << map_value(target_type_map_inverse, type) << ": " << token << ".\n";
                    const string target_name( token );
                    dependency_list dependencies( m_dependency_list );
                    read_dependency_list( dependencies );
                    if( error_status() )
                        return;

                    if( !next_token(token) && "{" == token )
                        return emit_error( "Expected '{' after " + map_value(target_type_map_inverse, type) + " target name." );

                    p_target = std::unique_ptr<target>(new target(target_name, type, dependencies) );

                    parse_target();
                    if( error_status() )
                        return;

                    targets.emplace_back( std::move(p_target) );
                }
            }
        }
        else if( "sub" == token )
        {
            debug(4) << "nectar_loader::extract_nectar::sub section found at line " << m_line_number << ".\n";
            // get name and dependencies of sub target
            if( next_token(token) )
            {
                const string sub_directory = s_ambrosia_config.source_directory()
                                             + directory_seperator + token;
                string sub_file( token + ".nectar.txt" );
                string sub_project_file( sub_directory + directory_seperator + sub_file );
                if( !file_exists(sub_project_file) )
                {
                    debug(4) << "nectar_loader::extract_nectar::sub target name and subproject file name do not match.\n";
                    sub_file = find_nectar_file( sub_directory );
                    if( error_status() )
                        return; // no *.nectar.txt file found

                    debug(4) << "nectar_loader::extract_nectar::found sub-.nectar.txt file: " << sub_file << ".\n";
                    sub_project_file = sub_directory + directory_seperator + sub_file;
                }
                debug(4) << "nectar_loader::extract_nectar::Opening subproject file: " << sub_project_file << ".\n";
                ifstream stream( sub_project_file );
                if( stream )
                {
                    dependency_list dependencies;
                    read_dependency_list( dependencies );
                    if( error_status() )
                        return;

                    nectar_loader sub_loader( sub_project_file, stream, dependencies );
                    sub_loader.extract_nectar( targets );
                    if( error_status() )
                        return;
                }
                else // opening file failed
                    return emit_error( "Error opening subproject file: " + sub_file + ". (line " + to_string(m_line_number) + ")" );
            }
            else
                return syntax_error( "\'sub\' must be followed by the name of the subproject." );
        }
        else
            return syntax_error( "Unexpected token: " + token + ". Expected global, app, lib, or sub." );
    }
    debug(3) << "nectar_loader::Finished with file: " << m_filename << ".\n";
}

void nectar_loader::syntax_error( const string &message ) const
{
    debug(4) << "nectar_loader::syntax_error::Emitting a syntax error here.\n";
    emit_error( "Syntax error: " + m_filename + ": line " + to_string(m_line_number) + "\n\t" + message );
}
void nectar_loader::syntax_warning( const string &message ) const
{
    debug(4) << "nectar_loader::syntax_warning::Emitting a syntax warning here.\n";
    emit_warning( "Syntax warning: " + m_filename + ": line " + to_string(m_line_number) + ": " + message );
}

bool nectar_loader::next_token( string &token, const std::set<char> &special_characters )
{
    // TODO: test the *full* hell out of this function
    // FIXME: ugly as hell, fixes welcome.
    //        - check for special char's in two places
    token.clear();
    bool inside_quotes = false;
    char c;

    while( m_stream.get(c) )
    {
        debug(7) << "nectar_loader::next_token::line number " << m_line_number
                 << ", character: \'" << output_form(c)
                 << "\', token so far: " << output_form(token) << "\n";
        if( inside_quotes )
        {
            debug(7) << "nectar_loader::next_token::Inside quotes.\n";
            if( '\"' == c )
                break; // end of token at end of quotes
            else if( '\n' == c )
            {
                syntax_error( "Quoted strings cannot span several lines." );
                return false;
            }
            else if( token.empty() && std::isspace(c, m_stream.getloc()) )
                syntax_error( "Beginning quote must not be followed by a whitespace." );
            else
                goto add_char;
        }
        else
        {
            if( token.empty() )
            {
                if( '\n' == c )
                    ++m_line_number;

                if( contains(special_characters, c) )
                {   // special characters are tokens of their own
                    debug(6) << "nectar_loader::next_token::Detected special character.\n";
                    token.append( 1, c );
                    return true;
                }
                else if( '\"' == c )
                {
                    debug(7) << "nectar_loader::next_token::Quote detected.\n";
                    inside_quotes = true;
                    continue;
                }
                else if( std::isspace(c, m_stream.getloc()) )
                    continue;
                else if( '#' == c )
                {   // skip over comments
                    debug(7) << "nectar_loader::next_token::Skipping over comments.\n";
                    string temp;
                    std::getline( m_stream, temp );
                    ++m_line_number;
                }
                else if( '\\' == c )
                {
                    string temp;
                    std::getline( m_stream, temp );
                    ++m_line_number;
                }
                else
                    goto add_char;
            }
            else if( std::isspace(c, m_stream.getloc()) || contains(special_characters, c) )
            {   // special characters or whitespace seperate tokens
                debug(6) << "nectar_loader::next_token::Detected special character or space.\n";
                m_stream.putback( c );
                break;
            }
            else if( '\"' == c )
            {
                syntax_error( "Beginning quotes must be preceded by a whitespace or a special character." );
                return false;
            }
            else
                add_char:
                token.append( 1, c );
        }
    }
    debug(6) << "nectar_loader::next_token:Token extracted: \'" << output_form(token) << "\'\n";
    return !token.empty();
}

void nectar_loader::read_dependency_list( dependency_list &dependencies )
{
    // copy "parent" dependencies
    dependencies = m_dependency_list;
    debug(4) << "nectar_loader::read_dependency_list::Reading dependencies.\n";
    bool in_list = false;
    target_type type;
    string token;
    while( next_token(token, s_special_characters_newline) )
    {
        debug(6) << "nectar_loader::read_dependency_list::token: " << token << ".\n";
        if( "{" == token || "\n" == token )
        {
            debug(4) << "nectar_loader::read_dependency_list::" << dependencies.size() << " dependencies.\n";
            return;
        }
        else if( !in_list )
        {
            if( ":" == token)
            {
                if( next_token(token) )
                {
                    if( "lib"==token )
                        type = target_type::lib;
                    else if( "app" == token )
                        type = target_type::app;

                    debug(6) << "nectar_loader::read_dependency_list::Detected " << token << " dependencies.\n";
                    in_list = true;
                    goto insert_dependency;
                }
                else
                    return syntax_error( "Expected list of target names after \':\'." );
            }
            else
                return syntax_error( "Expected target body \'{\' or dependency list \':\'." );
        }
        else if( "," == token )
        {
            insert_dependency:
            if( next_token(token) )
            {
                debug(6) << "nectar_loader::read_dependency_list::Inserting dependency: " << token << ".\n";
                const pair<target_type, string> element = { type, token };
                if( !dependencies.insert(element).second )
                {
                    if( !contains(m_dependency_list, element) )
                        return syntax_error( "Double dependency listed: "
                                             + map_value(target_type_map_inverse, type) + " " + token + "."  );
                }
            }
        }
        else
            in_list = false;
    }
}

const std::string nectar_loader::read_code_block()
{
    string block;
    size_t curly_brace_count = 1; // stream pointer starts inside of curly brace block
    size_t parenthesis_count = 0;
    bool inside_quotes = false;

    noskipws(m_stream);

    char current;
    for( istream_iterator<char> it(m_stream);
         curly_brace_count > 0  && m_stream;
         ++it )
    {
        current = *it;
        block.append( 1, current );
        debug(9) << "nectar_loader::read_code_block::Current character: \'" << current << "\'.\n";
        if( inside_quotes )
        {
            if( '\"' == current )
                inside_quotes = false;
        }
        else if( '\"' == current )
            inside_quotes = true;
        else if( '(' == current )
            ++parenthesis_count;
        else if( ')' == current )
        {
            if( parenthesis_count >= 1 )
                --parenthesis_count;
            else
            {
                syntax_error( "Too many closing parentheses \')\'." );
                break;
            }
        }
        else if( '{' == current )
            ++curly_brace_count;
        else if( '}' == current )
        {
            if( curly_brace_count >= 1 )
                --curly_brace_count;
            else
            {
                syntax_error( "Too many closing curly braces \'}\'." );
                break;
            }
        }
    }
    return block;
}
/*
 * Parsing
 **********/
bool nectar_loader::resolve_conditional( const std::function<bool(const string&)> &config_contains )
{
    bool result = true;

    // My very own recursive implementation
    /*
      - each set of parenthesis is handled recursively
      - logical AND: +
      - logical OR:  |
      - logical NOT: !
      - two bools: "result" and "current"
      - "result" keeps global result, and is modified by "+"
      - "current" keeps results for "|" and "!"
      - syntax checking for invalid
    */
    bool previous_was_operator = true;
    bool current = false; // keep track of current state
    string token;
    while( next_token(token) )
    {
        conditional_operator op;
        if( map_value(conditional_operator_map, token, op) )
        {
            if( previous_was_operator )
            {
                if( op == conditional_operator::not_op )
                    current ^= current; // negate next
                else
                {
                    syntax_error( "Conditional operators \'+\', \'|\', \')\', and \'(\' must be followed by a CONFIG string." );
                    break;
                }
            }
            else
            {
                switch( op )
                {
                    case conditional_operator::right_parenthesis:
                        // TODO: allow conditionals without outer parenthesis of the form (a+b)|(c)
                        return result;
                    case conditional_operator::left_parenthesis: // recurse
                        return resolve_conditional( config_contains );
                    case conditional_operator::plus_op:
                        result = result && current; // "current" -> "result"
                        if( !result ) // negative when an element of a "+" expression
                        current = false; // reset "current"
                        break;
                    case conditional_operator::or_op: // combine with "current"
                    throw runtime_error( "TODO" );
                    case conditional_operator::not_op: // unreachable
                        throw runtime_error( "Internal logic error in nectar_loader::resolve_conditional" );
                }
            }
            previous_was_operator = true;
        }
        else if( !previous_was_operator )
        {
            syntax_error( "In a conditional all CONFIG strings must be seperated by a conditional operator \'+\', \'|\', \')\', or \'(\'." );
            break;
        }
        else // previous was operator, so now we have a CONFIG string
        {
            // check CONFIG string, and perform a logical OR
            // TODO: check effect of "!"
            current = current || config_contains(token);
        }
    }


/*
    // (half of) Dijkstra's Shunting yard
    // Step 1: convert conditional expression to Reverse Polish notation
    // TODO fix implementation, is pretty shitty for now
    stack<conditional_operator> operator_stack;
    deque<string> output;

    string token;
    bool success = false;
    // Taken from http://en.wikipedia.org/wiki/Shunting-yard_algorithm#The_algorithm_in_detail
    while( next_token(token) )
    {
        conditional_operator op;
        if( map_value(conditional_operator_map, token, op) )
        {
            switch( op )
            {
                case conditional_operator::left_parenthesis:
                    operator_stack.push( op );
                    break;
                case conditional_operator::right_parenthesis:
                    while( !operator_stack.empty() && operator_stack.top() != conditional_operator::left_parenthesis )
                    {
                        output.push_back( map_value(conditional_operator_map_inverse, operator_stack.top()) );
                        operator_stack.pop();
                    }
                    if( operator_stack.empty() )
                    {
                        syntax_error( "No matching right parenthesis in conditional." );
                        goto end;
                    }
                    break;
                default: // real operators: + | !
                    while( !operator_stack.empty() )
                    {
                        // Check precedence by comparing precedence ordered enum values
                        if( op <= operator_stack.top() )
                        {
                            output.push_back( map_value(conditional_operator_map_inverse, operator_stack.top()) );
                            operator_stack.pop();
                        }
                    }
            }
        }
        else // token is neither op ("+","|" or "!") or parenthesis
            output.push_back( token );
    }
    end:
    // check success
    if( !success )
        emit_error( "Failed to parse conditional." );
*/
    return result;
}

bool nectar_loader::process_outer_conditional()
{
    syntax_error( "Outer conditionals not implemented yet." );
    return false;
}
bool nectar_loader::process_dependency_list_conditional()
{
    syntax_error( "Outer list conditionals not implemented yet." );
    return false;
}

bool nectar_loader::process_inner_conditional()
{
    syntax_error( "Inner conditionals not implemented yet." );
    return false;
}
bool nectar_loader::process_inner_list_conditional()
{
    syntax_error( "Inner list conditionals not implemented yet." );
    return false;
}

bool nectar_loader::parse_list( function<bool(const string &)> validate,
                                function<const string_set(const string &)> insert,
                                function<const string_set(const string &)> remove )
{
    debug(4) << "nectar_loader::parse_list::Parsing list.\n";
    size_t curly_braces_count = 0;
    string token;
    bool list_empty = true;
    while( next_token(token, s_special_characters_newline) )
    {
        debug(4) << "nectar_loader::parse_list::token: " << token << ".\n";
        if( "\n" == token )
            break; // list has ended
        else if( "(" == token )
        {
            if( !process_inner_list_conditional() )
                return false;
        }
        else if( "}" == token )
        {
            if( curly_braces_count > 0 )
                curly_braces_count--;
            else
            {
                syntax_error( "Unexpected closing curly brace." );
                return false;
            }
        }
        else if( "~" == token && next_token(token, s_special_characters_newline) )
        {
            debug(5) << "nectar_loader::parse_list::removing item from list: " << token << ".\n";
            if( !validate(token) )
                return false;

            list_empty = false;
            const string_set result = remove( token );
            switch( result.size() )
            {
                case 0:
                    break; // all OK, continue
                case 1:
                    syntax_warning( "Item that is not in list cannot be deleted: " + token );
                    break;
                default:
                    syntax_error( "Could not delete ambiguous item: " + token + "\n"
                                  + "\tAmbiguous with:");
                    std::for_each( result.begin(), result.end(),
                                   [this](const string &item)
                                   {
                                       syntax_error( "\t" + item );
                                   } );
            }
        }
        else
        {
            debug(5) << "nectar_loader::parse_list::adding item to list: " << token << ".\n";
            if( !validate(token) )
                return false;

            list_empty = false;
            const string_set result = insert( token );
            switch( result.size() )
            {
                case 0:
                    break; // all OK, continue
                case 1:
                    syntax_error( "Duplicate item: " + *result.begin() );
                    break;
                default:
                    syntax_error( "Ambiguous item: " + token );
                    std::for_each( result.begin(), result.end(),
                               [this](const string &item)
                               {
                                   syntax_error( "\t" + item );
                               } );
            }
        }
    }
    debug(4) << "nectar_loader::parse_list::Done with list.\n";
    if( curly_braces_count > 0 )
        syntax_error( "Unclosed curly braces." );
    else if( list_empty )
        syntax_error( "A list must not be empty. Place the conditional before the list name." );

    // gather any errors and make sure parsing does not continue
    if( error_status() )
        return false;
    else
        return true;
}

void nectar_loader::parse_target()
{
    const target_type type = p_target->type();
    const std::string target_name( p_target->name() );
    debug(4) << "nectar_loader::parse_binary_or_global::Processing named target section: " << target_name << ".\n";
    size_t curly_brace_count = 1; // parsing starts inside curly braces block
    string token;
    bool already_modified_NAME = false;

    while( curly_brace_count > 0 && next_token(token) )
    {
        debug(4) << "nectar_loader::parse_binary_or_global::token: " << token << ".\n";
        if( "}" == token )
            --curly_brace_count;
        else if( "{" == token )
            ++curly_brace_count;
        else if( "(" == token )
        {
            if( !process_inner_conditional() )
                return; // emit_error already called
        }
        else if( "CONFIG" == token)
        {
            debug(5) << "nectar_loader::parse_global::CONFIG detected.\n";
            if( !parse_list(std::bind(&nectar_loader::validate_CONFIG, this, _1),
                            std::bind(&target::add_config, p_target.get(), _1),
                            std::bind(&target::remove_config, p_target.get(), _1)) )
                return; // failure
        }
        else if ( "NAME" == token )
        {
            debug(5) << "nectar_loader::parse_target::NAME detected.\n";
            if( type == target_type::global )
                return syntax_error( "global target does not need a name");

            if( already_modified_NAME )
                syntax_warning( "NAME is being modified twice in this target section." );

            already_modified_NAME = true;
            if( next_token(token) )
                p_target->set_output_name( token );
            else
                return syntax_error( "NAME must be followed by the target's output name (without prefix/suffix" );
        }
        else
        {
            file_type type;
            // is it a list of files?
            if( map_value(file_type_map, token, type) )
            {
                debug(5) << "nectar_loader::parse_target::" << token << " file list detected.\n";
                if( !parse_list(std::bind(&nectar_loader::validate_filename, this, _1),
                                std::bind(&target::add_files, p_target.get(), type, _1),
                                std::bind(&target::remove_files, p_target.get(), type, _1)) )
                {
                    debug(6) << "nectar_loader::parse_target::Failed at inserting file(s).\n";
                    return; // failure, assumes parse_list has called emit_error
                }
            } // or a list of directories
            else if( map_value(directory_type_map, token, type) )
            {
                debug(5) << "nectar_loader::parse_target::" << map_value(file_type_map_inverse, type) << " directory list detected.\n";
                if( !parse_list(std::bind(&nectar_loader::validate_directory, this, _1),
                                std::bind(&target::add_directories, p_target.get(), type, _1),
                                std::bind(&target::remove_directories, p_target.get(), type, _1)) )
                {
                    debug(6) << "nectar_loader::parse_target::Failed at inserting directory/directories.\n";
                    return; // failure, assumes parse_list has called emit_error
                }
            }
            else
                return syntax_error( "Unexpected token: " + token );
        }
    }
}

bool nectar_loader::validate_CONFIG( const string &config )
{
    return ( config.find_first_of("*?/$") == string::npos );
}
bool nectar_loader::validate_filename( const string &filename )
{
    const size_t index = filename.find_first_of( "/?*" );
    if( index != string::npos && '/' != filename[index])
    {   // there might be wildcards before a directory seperator
        if( filename.find('/',index+1) != string::npos )
        {
            syntax_error( "Wildcard characters ?* are not allowed in directory names." );
            return false;
        }
    }
    return true;
}
bool nectar_loader::validate_directory( const string &directory )
{
    if( !(directory.find_first_of("*?") == string::npos) )
    {
        syntax_error( "Wildcard characters ?* are not allowed in directory names." );
        return false;
    }
    else
        return true;
}

libambrosia_namespace_end
