#pragma once
#include "../string.hpp"
#include <stdio.h>
#include <stdint.h>

namespace cpplib
{
namespace filesystem
{
enum class seek_start_point
	{
	beginning,
	end,
	current
	};

enum class entry_type
	{
	none,
	file,
	folder,
	link
	};

enum class file_open_mode
	{
	read_only,
	write_truncate,
	append,
	read_update,
	write_update,
	append_update
	};

class entry
	{
	private:
		FILE *file_handle;
		string filename;
		file_open_mode mode;
		size_t size;
		entry_type type;
	public:
		entry( void );
		~entry( void );
		bool set( const char *path );
		bool open( const file_open_mode new_mode = file_open_mode::read_only );
		bool close( void );
		bool write( const uint8_t *data, const size_t length, size_t *bytes_written =nullptr);
		bool read( const uint8_t *data, const size_t length, size_t *bytes_read =nullptr);
		bool seek( const off_t offset, const seek_start_point start_point );
		bool tell( uint64_t &offset );

		entry_type get_type( void ) const;
		size_t get_size( void ) const;
	};
}
}