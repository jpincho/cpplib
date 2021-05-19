#pragma once
#include "string.hpp"
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

class file
	{
	public:
		enum class file_open_mode
			{
			read_only,
			write_truncate,
			append,
			read_update,
			write_update,
			append_update
			};
	private:
		FILE *file_handle;
		string filename;
		file_open_mode mode;
	public:
		file( void );
		~file( void );
		bool open( const char *filename, const file_open_mode new_mode = file_open_mode::read_only );
		bool close( void );
		bool write( const uint8_t *data, const size_t length, size_t *bytes_written =nullptr);
		bool read( const uint8_t *data, const size_t length, size_t *bytes_read =nullptr);
		bool seek( const off_t offset, const seek_start_point start_point );
		bool tell( uint64_t &offset );
	};
}
}