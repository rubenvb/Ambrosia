/**
  * Ambrosia/file_store.cpp
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class include
#include "file_store.h"

// libAmbrosia includes
#include "algorithm.h"
#include "ambrosia_config.h"
#include "debug.h"
#include "platform.h"
#include "status.h"

// C++ includes
#include <stdexcept>
    using std::logic_error;
/* <string> */
    using std::string;
/* <utility> */
    using std::pair;

libambrosia_namespace_begin

file_store s_file_store; // declared in global.h

file_store::file_store()
:   m_source_files(),
    m_build_files()
{   }

const file_set & file_store::get_source_file_set( const std::string &directory )
{
    const auto result = m_source_files.find( directory );
    if( result != m_source_files.end() )
        return (*result).second;

    add_source_directory( directory );

    return get_source_file_set( directory );
}

const string_set file_store::find_source_file( const string_set &directories, const string &filename )
{
    debug(4) << "file_store::find_source_file::Looking for " << filename << "in all source directories.\n";
    string_set result;
    const string_pair directory_filename( split_preceding_directory(filename) );
    const string &preceding_directory( directory_filename.first );
    const string &true_filename( directory_filename.second );

    const auto end = directories.end();
    for( auto it = directories.begin(); it != end; ++it )
    {
        const string directory( *it + directory_seperator + preceding_directory );

        const file_set &files_on_disk = get_source_file_set( directory );
        if( error_status() )
            return result;

        const auto end = files_on_disk.end();
        for( auto it = files_on_disk.begin(); it != end; ++it )
        {
            const file &entry = *it;
            if( entry.first == true_filename )
                result.insert( {directory + directory_seperator + true_filename, entry.second} );
        }
    }
    return result;
}
const file_set file_store::match_source_files( const string_set &directories, const string &filename )
{
    debug(4) << "file_store::match_source_files::Matching " << filename << " to all files in the source directories.\n";
    file_set result;
    const string_pair directory_filename( split_preceding_directory(filename) );
    const string &preceding_directory( directory_filename.first );
    const string &true_filename( directory_filename.second );

    // search all directories, appended with preceding_directory
    const auto directory_end = directories.end();
    for( auto directory_it = directories.begin(); directory_it != directory_end; ++directory_it )
    {
        const string directory( *directory_it + directory_seperator + preceding_directory );
        debug(5) << "file_store::match_source_files::Looking in " << directory << " for matches.\n";

        const file_set &files_on_disk = get_source_file_set( directory );
        if( error_status() )
            return result;

        // match all files that were scanned from disk to the wildcard filename
        const auto files_end = files_on_disk.end();
        for( auto files_it = files_on_disk.begin(); files_it != files_end; ++files_it )
        {
            const file &entry = *files_it; // filename and last moddified time
            if( wildcard_compare(true_filename, entry.first) )
                result.insert( { directory + directory_seperator + entry.first, entry.second } );
        }
    }
    return result;
}

void file_store::add_source_directory( const std::string &directory )
{
    const string full_path( s_ambrosia_config.source_directory()+ directory_seperator + directory );
    if( !directory_exists(directory) )
        return emit_error( "Directory does not exist: " + full_path );

    debug(5) << "file_store::add_source_directory::Scanning files in source directory: " << full_path << ".\n";
    const auto result = m_source_files.insert( {directory, file_set()} );
    if( !result.second )
        debug(5) << "file_store::add_source_directory::Directory already present, and scanned.\n";
    else
    {
        file_set &entries = (*result.first).second;
        scan_directory( std::inserter(entries, entries.begin()), full_path );
    }
}
void file_store::add_build_directory( const std::string &directory )
{
    const string full_path( s_ambrosia_config.build_directory() + directory_seperator + directory );
    if( !directory_exists(directory) )
        return emit_error( "Directory does not exist: " + full_path );

    debug(5) << "file_store::add_build_directory::Scanning files in build directory: " << full_path << ".\n";
    const auto result = m_build_files.insert( {directory, file_set()} );
    if( !result.second )
        debug(5) << "file_store::add_source_directory::Directory already present, and scanned.\n";
    else
    {
        file_set &entries = (*result.first).second;
        scan_directory( std::inserter(entries, entries.begin()), full_path );
    }
}

libambrosia_namespace_end
