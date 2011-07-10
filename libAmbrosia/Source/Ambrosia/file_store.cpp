/**
  * Ambrosia/file_store.cpp
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class include
#include "Ambrosia/file_store.h"

// libAmbrosia includes
#include "Ambrosia/algorithm.h"
#include "Ambrosia/Configuration/ambrosia_config.h"
#include "Ambrosia/debug.h"
#include "Ambrosia/platform.h"
#include "Ambrosia/status.h"

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
    debug(5) << "file_set::get_source_file_set::Finding directory listing for "
             << s_ambrosia_config.source_directory() << directory << ".\n";
    const auto result = m_source_files.find( directory );
    if( result != m_source_files.end() )
        return m_source_files[directory];
    else
    {
        debug(5) << "file_store::get_source_file_set::Directory not found, scanning now.\n";
        add_source_directory( directory );
        return m_source_files[directory];
    }
}

const file_set file_store::find_source_file( const string &filename, const string_set &directories )
{
    string_set directories_to_search = directories;
    debug(4) << "file_store::find_source_file::Looking for " << filename
             << " in subdirectories of: " <<  s_ambrosia_config.source_directory() << "\n"
             << directories;

    file_set result;
    const string_pair directory_filename( split_preceding_directory(filename) );
    const string &preceding_directory = directory_filename.first;
    const string &true_filename = directory_filename.second;

    // handle empty "directories" case
    if( directories.empty() )
        directories_to_search.insert("");

    const auto end = directories_to_search.end();
    for( auto it = directories_to_search.begin(); it != end; ++it )
    {
        const string directory = *it + preceding_directory;

        debug(5) << "file_store::find_source_file::Loading directory contents for: "
                 << s_ambrosia_config.source_directory() << "/" << directory << ".\n";
        const file_set &files_on_disk = get_source_file_set( directory );
        if( error_status() )
            return result;

        const auto end = files_on_disk.end();
        for( auto it = files_on_disk.begin(); it != end; ++it )
        {
            const file &entry = *it;
            debug(5) << "file_store::find_source_file::Matching " << entry.first << " vs " << true_filename << ".\n";
            if( wildcard_compare(true_filename, entry.first) )
            {
                debug(5) << "file_store::find_source_file::Match found: " << entry.first << "\n";
                result.insert( {directory + "/" + entry.first, entry.second} );
            }
        }
    }
    debug(4) << "file_store::find_source_file::Found " << result.size() << " match(es).\n";
    return result;
}
const file_set file_store::match_source_files( const string &filename, const string_set &directories )
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
        const string directory( *directory_it + preceding_directory );
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
                result.insert( { directory + "/" + entry.first, entry.second } );
        }
    }
    return result;
}

void file_store::add_source_directory( const std::string &directory )
{
    if( !directory_exists(directory) )
    {
        debug(5) << "file_store::add_source_directory::Non-existing directory: " << directory << "\n";
        return emit_error( "Directory does not exist: " + directory );
    }

    debug(5) << "file_store::add_source_directory::Scanning files in source directory: " << directory << ".\n";
    const auto result = m_source_files.insert( {directory, file_set()} );
    if( !result.second )
        debug(5) << "file_store::add_source_directory::Directory already present, and scanned.\n";
    else
    {
        file_set &new_files = (*result.first).second;
        scan_directory( std::inserter(new_files, new_files.begin()), directory );
        debug(5) << "file_store::add_source_directory::Directory scanned.\n";
    }
}
void file_store::add_build_directory( const std::string &directory )
{
    if( !directory_exists(directory) )
        // TODO: Create directory so scan_directory works OK. Check if creation was possible.
        debug(5) << "file_store::add_build_direcctory::Creating build directory: " << directory << ".\n";

    debug(5) << "file_store::add_build_directory::Scanning files in build directory: " << directory << ".\n";
    const auto result = m_build_files.insert( {directory, file_set()} );
    if( !result.second )
        debug(5) << "file_store::add_source_directory::Directory already present, and scanned.\n";
    else
    {
        file_set &new_files = (*result.first).second;
        scan_directory( std::inserter(new_files, new_files.begin()), directory );
    }
}

libambrosia_namespace_end
