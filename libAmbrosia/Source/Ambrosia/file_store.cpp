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
#include "ambrosia_config.h"
#include "debug.h"
#include "platform.h"
#include "status.h"

// C++ includes
#include <stdexcept>
    using std::logic_error;
/* <string> */
    using std::string;

libambrosia_namespace_begin

file_store s_file_store; // declared in global.h

file_store::file_store()
:   m_source_files(),
    m_build_files()
{   }

void file_store::match_source_files( const string_set &directories, const std::string &filename,
                                     file_set &matches, file_set &duplicates )
{
    const auto end = directories.end();
    for( auto it = directories.begin(); it != end; ++it )
    {
        const string &directory = *it;
        auto result = m_source_files.find(directory);
        if( result != m_source_files.end() )
            const file_set &files_on_disk = (*result).second;
        else
            throw std::logic_error( "Internal logic error in file_store::match_source_files: directory "
                                    + directory + "not added to s_file_store but present in target." );

    }


}

void file_store::add_source_directory( const std::string &directory )
{
    const string full_path( s_ambrosia_config.source_directory()+ directory_seperator + directory );
    if( !directory_exists(directory) )
        return emit_error( "Directory does not exist: " + full_path );

    debug(5) << "file_store::add_source_directory::Scanning files in source directory: " << full_path << ".\n";
    const auto result = m_source_files.insert( {directory, file_set()} );
    if( !result.second )
    {
        debug(5) << "file_store::add_source_directory::Directory already present, and scanned.\n";
        return;
    }
    file_set &entries = (*result.first).second;
    scan_directory( std::inserter(entries, entries.begin()), full_path );
}
void file_store::add_build_directory( const std::string &directory )
{
    const string full_path( s_ambrosia_config.build_directory() + directory_seperator + directory );
    if( !directory_exists(directory) )
        return emit_error( "Directory does not exist: " + full_path );

    debug(5) << "file_store::add_build_directory::Scanning files in build directory: " << full_path << ".\n";
    const auto result = m_build_files.insert( {directory, file_set()} );
    if( !result.second )
    {
        debug(5) << "file_store::add_source_directory::Directory already present, and scanned.\n";
        return;
    }
    file_set &entries = (*result.first).second;
    scan_directory( std::inserter(entries, entries.begin()), full_path );
}


libambrosia_namespace_end
