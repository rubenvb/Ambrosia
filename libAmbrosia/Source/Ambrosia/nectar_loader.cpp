/**
  * Parser/nectar_loader.h
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class include
#include "Ambrosia/nectar_loader.h"

// libAmbrosia includes
#include "Ambrosia/algorithm.h"
#include "Ambrosia/Configuration/ambrosia_config.h"
#include "Ambrosia/debug.h"
#include "Ambrosia/global.h"
#include "Ambrosia/enums.h"
#include "Ambrosia/enum_maps.h"
#include "Ambrosia/nectar.h"
#include "Ambrosia/platform.h"
#include "Ambrosia/status.h"
#include "Ambrosia/target.h"

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

nectar_loader::nectar_loader( const string &filename, const string &directory,
                              istream &stream, const dependency_list &list )
:   m_filename( filename ),
    m_directory( directory ),
    m_stream( stream ),
    m_line_number( 1 ),
    m_dependency_list( list ),
    m_global_processed( false ),
    p_target()
{
    debug(0) << "nectar_loader::nectar_loader::filename is " << filename << "\n";
}
nectar_loader::~nectar_loader()
{
    if( p_target != nullptr )
        debug(0) << "nectar_loader::" << p_target->name() + " in file " + m_filename + " not pushed to a target_list.\n";
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
                return emit_syntax_error( "Second global section found in nectar file. Only one global section per *.nectar.txt file is allowed." );

            if( next_token(token) && "{" == token )
            {
                p_target = std::unique_ptr<target>( new target("global", target_type::global, dependency_list()) );

                parse_target();
                if( error_status() )
                    return;

                targets.emplace_back( std::move(p_target) );
            }
            else
                return emit_syntax_error( "\'global\' must be followed by \'{\'." );
        }
        else if( "app" == token || "lib" == token )
        {
            const target_type type( (*target_type_map.find(token)).second );
            debug(4) << "nectar_loader::extract_nectar::" << token << " section found at line " << m_line_number << ".\n";
            if( next_token(token) )
            {
                if( "{" == token )
                    return emit_nectar_error( "Syntax error: Expected " + token + " name after " + token + "." );
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
                                             + "/" + token;
                string sub_file( token + ".nectar.txt" );
                string sub_project_file( sub_directory + "/" + sub_file );
                if( !file_exists(sub_project_file) )
                {
                    debug(4) << "nectar_loader::extract_nectar::sub target name and subproject file name do not match.\n";
                    sub_file = find_nectar_file( sub_directory );
                    if( error_status() )
                        return; // no *.nectar.txt file found

                    debug(4) << "nectar_loader::extract_nectar::found sub-.nectar.txt file: " << sub_file << ".\n";
                    sub_project_file = sub_directory + "/" + sub_file;
                }
                debug(4) << "nectar_loader::extract_nectar::Opening subproject file: " << sub_project_file << ".\n";
                auto stream_ptr = open_ifstream( sub_project_file );
                auto &stream = *stream_ptr;
                if( stream )
                {
                    // Get sub target dependencies
                    dependency_list dependencies;
                    read_dependency_list( dependencies );
                    if( error_status() )
                        return;

                    // get subdirectory
                    string subdirectory;
                    if( m_directory.empty() )
                        subdirectory = sub_directory;
                    else
                        subdirectory = m_directory + "/" + sub_directory;

                    nectar_loader sub_loader( sub_project_file, subdirectory, stream, dependencies );
                    sub_loader.extract_nectar( targets );
                    if( error_status() )
                        return;
                }
                else // opening file failed
                    return emit_nectar_error( "Error opening subproject file: " + sub_file + "." );
            }
            else
                return emit_syntax_error( "\'sub\' must be followed by the name of the subproject." );
        }
        else
            return emit_syntax_error( "Unexpected token: " + token + ". Expected global, app, lib, or sub." );
    }
    debug(3) << "nectar_loader::Finished with file: " << m_filename << ".\n";
}
/*
 * Error/warning reporting
 **************************/
void nectar_loader::emit_nectar_error( const std::string &message ) const
{
    libambrosia::emit_nectar_error( message, m_filename, m_line_number );
}
void nectar_loader::emit_nectar_warning( const std::string &message ) const
{
    libambrosia::emit_nectar_warning( message, m_filename, m_line_number );
}
void nectar_loader::emit_syntax_error( const std::string &message ) const
{
    libambrosia::emit_nectar_error( "Syntax error: " + message, m_filename, m_line_number );
}
void nectar_loader::emit_syntax_warning( const std::string &message ) const
{
    libambrosia::emit_nectar_warning( "Syntax warning: " + message, m_filename, m_line_number );
}
/*
 * Lexing
 *********/
bool nectar_loader::next_token( string &token, const std::set<char> &special_characters )
{
    // TODO: test the *full* hell out of this function
    // FIXME: ugly as hell, alternatives welcome.
    token.clear();
    bool inside_quotes = false;
    char c;

    while( m_stream.get(c) )
    {
        debug(8) << "nectar_loader::next_token::line number " << m_line_number
                 << ", character: \'" << output_form(c)
                 << "\', token so far: " << output_form(token) << "\n";
        if( inside_quotes )
        {
            debug(9) << "nectar_loader::next_token::Inside quotes.\n";
            if( '\"' == c )
                break; // end of token at end of quotes
            else if( '\n' == c )
            {
                emit_syntax_error( "Quoted strings cannot span several lines." );
                return false;
            }
            else if( token.empty() && std::isspace(c, m_stream.getloc()) )
                emit_syntax_error( "Beginning quote must not be followed by a whitespace." );
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
                    debug(9) << "nectar_loader::next_token::Quote detected.\n";
                    inside_quotes = true;
                    continue;
                }
                else if( std::isspace(c, m_stream.getloc()) )
                    continue;
                else if( '#' == c )
                {   // skip over comments
                    debug(8) << "nectar_loader::next_token::Skipping over comments.\n";
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
                debug(7) << "nectar_loader::next_token::Detected special character or space.\n";
                m_stream.putback( c );
                break;
            }
            else if( '\"' == c )
            {
                emit_syntax_error( "Beginning quotes must be preceded by a whitespace or a special character." );
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
bool nectar_loader::next_list_token( std::string &token )
{
    debug(4) << "nectar_loader::next_list_token::reading next list item.\n";
    size_t curly_braces_count = 0;
    while( next_token(token, s_special_characters_newline) )
    {
        debug(4) << "nectar_loader::next_list_token::token: " << output_form(token) << ".\n";
        if( "\n" == token )
            return false; // list has ended
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
                emit_syntax_error( "Unexpected closing curly brace." );
                return false;
            }
        }
        else // normal list item
            return true;

    }
    if( curly_braces_count > 0 )
    {
        emit_syntax_error( "Unclosed curly braces in list." );
        return false;
    }
    return true;
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
        debug(6) << "nectar_loader::read_dependency_list::token: " << output_form(token) << ".\n";
        if( "{" == token || "\n" == token )
        {
            debug(4) << "nectar_loader::read_dependency_list::" << dependencies.size() << " dependencies.\n";
            return;
        }
        else if( !in_list )
        {
            if( ":" == token )
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
                    return emit_syntax_error( "Expected list of target names after \':\'." );
            }
            else
                return emit_syntax_error( "Expected target body \'{\' or dependency list \':\'." );
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
                        return emit_nectar_error( "Double dependency listed: "
                                                  + map_value(target_type_map_inverse, type) + " "
                                                  + token + "."  );
                }
            }
        }
        else
            in_list = false;
    }
}
/*
 * Parsing
 **********/
bool nectar_loader::test_condition( const std::function<bool(const string&)> &config_contains )
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
            debug(6) << "nectar_loader::test_condition::operator found: " << token << ".\n";
            if( previous_was_operator )
            {
                if( op == conditional_operator::not_op )
                    current ^= current; // negate next
                else
                {
                    emit_syntax_error( "Conditional operators \'+\', \'|\', \')\', and \'(\' must be followed by a CONFIG string." );
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
                        return test_condition( config_contains );
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
            emit_syntax_error( "In a conditional all CONFIG strings must be seperated by a conditional operator \'+\', \'|\', \')\', or \'(\'." );
            break;
        }
        else // previous was operator, so now we have a CONFIG string
        {
            // check CONFIG string, and perform a logical OR
            // TODO: check effect of "!"
            current = current || config_contains(token);
            previous_was_operator = false;
        }
    }
    return result;
}

bool nectar_loader::process_outer_conditional()
{
    emit_nectar_error( "Outer conditionals not implemented yet." );
    return false;
}
bool nectar_loader::process_dependency_list_conditional()
{
    emit_nectar_error( "Outer list conditionals not implemented yet." );
    return false;
}

bool nectar_loader::process_inner_conditional()
{
    if( test_condition( [this](const string &item){ return contains(p_target->config().config(), item); }) )
        debug(4) << "nectar_loader::process_inner_conditional::condition returned true, nothing to skip.\n";
    else
    {
        debug(4) << "nectar_loader::process_inner_conditional::conditional returned false, skipping all relevant parts.\n";
    }

    emit_error( "!!!Inner conditionals not fully implemented yet.\n" );

    return !error_status();
}
bool nectar_loader::process_inner_list_conditional()
{
    emit_nectar_error( "Inner list conditionals not implemented yet." );
    return false;
}

bool nectar_loader::parse_file_list( const file_type type )
{
    bool empty = true; // a list must not be empty
    string token;

    while( next_list_token(token) )
    {
        debug(6) << "nectar_loader::parse_file_list::adding matches to file " << token << " to the file list.\n";
        empty = false;
        p_target->add_source_file(type, token, m_filename, m_line_number); // errors are assembled in this function
    }
    if( empty )
        emit_syntax_error( "A list must not be empty." );

    return !error_status();
}
bool nectar_loader::parse_source_directory_list( const file_type type )
{
    bool empty = true; // a list must not be empty
    string token;
    const string &source_directory = p_target->config().source_directory();
    // gather all list items
    while( next_list_token(token) )
    {
        debug(6) << "nectar_loader::parse_source_directory_list::Checking if directory exists: "
                 << source_directory << "/" << full_directory_name(m_directory, token) << ".\n";
        empty = false;
        if( !p_target->add_source_directory(type, full_directory_name(m_directory, token)) )
            emit_error_list( {token + "(line " + to_string(m_line_number) + ")"} ); // add the bad directory to error_list
    }
    if( empty )
        emit_syntax_error( "A list must not be empty" );

    return !error_status();
}
bool nectar_loader::parse_build_directory( const file_type )
{
    emit_nectar_error( "Build directory list parsing isn't done yet." );
    return false;
}
bool nectar_loader::parse_variable_list( string_set & )
{
    emit_nectar_error( "Variable list parsing isn't done yet." );
    return false;
}

void nectar_loader::parse_target()
{
    const target_type type = p_target->type();
    const std::string target_name( p_target->name() );
    debug(4) << "nectar_loader::parse_target::Processing named target section: " << target_name << ".\n";
    size_t curly_brace_count = 1; // parsing starts inside curly braces block
    string token;
    bool already_modified_NAME = false;

    while( curly_brace_count > 0 && next_token(token) )
    {
        debug(4) << "nectar_loader::parse_target::token: " << token << ".\n";
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
            debug(5) << "nectar_loader::parse_target::CONFIG detected.\n";
            if( !parse_variable_list(p_target->config().config()) )
                return; // failure
        }
        else if ( "NAME" == token )
        {
            debug(5) << "nectar_loader::parse_target::NAME detected.\n";
            if( type == target_type::global )
                return emit_syntax_error( "global target must not have a name");

            if( already_modified_NAME )
                emit_syntax_warning( "NAME is being modified twice in this target section." );

            already_modified_NAME = true;
            if( next_token(token, s_special_characters_newline) )
            {
                if( "\n" != token )
                {
                    p_target->set_output_name( token );
                    continue;
                }
            }
            return emit_syntax_error( "NAME must be followed by the target's output name (without prefix/suffix" );
        }
        else
        {
            file_type type;
            // is it a list of files?
            if( map_value(file_type_map, token, type) )
            {
                debug(5) << "nectar_loader::parse_target::" << token << " file list detected.\n";
                if( !parse_file_list(type) )
                {
                    debug(6) << "nectar_loader::parse_target::Failed parsing file list.\n";
                    return; // failure, assumes parse_file_list has called emit_error
                }
            } // or a list of directories
            else if( map_value(directory_type_map, token, type) )
            {
                debug(5) << "nectar_loader::parse_target::" << map_value(file_type_map_inverse, type) << " directory list detected.\n";
                const file_type general_type = get_general_type( type );
                if( general_type == file_type::source || type == file_type::header )
                {
                    if( !parse_source_directory_list(type) )
                    {
                        debug(6) << "nectar_loader::parse_target::Failed at parsing source directory/directories.\n";
                        return; // failure, assumes parse_source_directory_list has called emit_error
                    }
                }
                else
                {
                    // parse other types of directories
                }
            }
            else
                return emit_syntax_error( "Unexpected token: " + token );
        }
    }
}

bool nectar_loader::validate_variable( const string &config )
{
    return ( config.find_first_of("*?/$") == string::npos );
}
bool nectar_loader::validate_filename( const string &filename )
{
    const string::size_type index = filename.find_first_of( "/?*" );
    if( index != string::npos && '/' != filename[index])
    {   // there might be wildcards before a directory seperator
        if( filename.find('/',index+1) != string::npos )
        {
            emit_syntax_error( "Directory seperators '/' are not allowed in directory names." );
            return false;
        }
    }
    return true;
}
bool nectar_loader::validate_directory( const string &directory )
{
    if( !(directory.find_first_of("*?") == string::npos) )
    {
        emit_syntax_error( "Wildcard characters ?* are not allowed in directory names." );
        return false;
    }
    else
        return true;
}

libambrosia_namespace_end
