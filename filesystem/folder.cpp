#include "../cpplib_config.h"
#include "folder.hpp"
#include <sys/stat.h>
#if defined ( CPPLIB_TARGET_PLATFORM_WINDOWS )
#include <windows.h>
#else
#include <dirent.h>
#include <unistd.h>
#endif

namespace cpplib
{
namespace filesystem
{
folder::folder( void )
	{
	}

folder::~folder( void )
	{
	}

bool folder::set( const char *new_foldername )
	{
	foldername = new_foldername;
	return true;
	}

bool folder::list( array<string> &entries, const bool recursive )
	{
#if defined ( CPPLIB_TARGET_PLATFORM_WINDOWS )
	WIN32_FIND_DATA find_data;
	memset( &find_data, 0, sizeof( find_data ) );
	HANDLE find_handle = INVALID_HANDLE_VALUE;
	// FindFirstFile needs a mask, so add it here... Stupid Windows...
	find_handle = FindFirstFile( ( foldername + "/*" ).get_data(), &find_data );
	if ( find_handle == INVALID_HANDLE_VALUE ) // cannot open folder for listing
		return false;

	do
		{
		entries.push_back( find_data.cFileName );
		}
	while ( FindNextFile( find_handle, &find_data ) != 0 );
	FindClose( find_handle );

#else
	struct dirent *find_data = nullptr;
	DIR *folder_handle = nullptr;

	folder_handle = opendir( foldername.get_data() );
	if ( folder_handle == nullptr )
		return false;

	while ( ( find_data = readdir( folder_handle ) ) != nullptr )
		{
		entries.push_back( find_data->d_name );
		/*
		if ( ( recursive ) && ( find_data->d_type == DT_DIR ) && ( strcmp( find_data->d_name, "." ) != 0 ) && ( strcmp( find_data->d_name, ".." ) != 0 ) )
			{
			std::vector <std::string> child_entries;
			std::string current_topmost( find_data->d_name );
			list_folder( folder_path + current_topmost, child_entries, true );
			for ( size_t child_iterator = 0; child_iterator < child_entries.size(); ++child_iterator )
				entries.push_back( ( filesystem::path( current_topmost ) + child_entries[child_iterator] ).get_full() );
			}*/
		}
	closedir( folder_handle );
#endif
	}
}
}