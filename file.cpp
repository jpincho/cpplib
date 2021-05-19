#include "file.hpp"

namespace cpplib
{
namespace filesystem
{
file::file( void )
	{
	file_handle = nullptr;
	}

file::~file( void )
	{
	if ( file_handle )
		close();
	}

bool file::open( const char *filename, const file_open_mode new_mode )
	{
	if ( file_handle )
		return false;

	char mode_text[3];
	uint8_t mode_text_length = 0;

	switch ( new_mode )
		{
		case file_open_mode::read_only:
			mode_text[mode_text_length++] = 'r'; 
			break;
		case file_open_mode::write_truncate:
			mode_text[mode_text_length++] = 'w'; 
			break;
		case file_open_mode::append:
			mode_text[mode_text_length++] = 'a'; 
			break;
		case file_open_mode::read_update:
			mode_text[mode_text_length++] = 'r';
			mode_text[mode_text_length++] = '+'; 
			break;
		case file_open_mode::write_update:
			mode_text[mode_text_length++] = 'w';
			mode_text[mode_text_length++] = '+'; 
			break;
		case file_open_mode::append_update:
			mode_text[mode_text_length++] = 'a';
			mode_text[mode_text_length++] = '+'; 
			break;
		}
	mode_text[mode_text_length] = 0;
	file_handle = fopen( filename, mode_text );
	if ( !file_handle )
		return false;
	this->filename = filename;
	mode = new_mode;
	return true;
	}

bool file::close( void )
	{
	if ( !file_handle )
		return false;
	fclose( file_handle );
	file_handle = nullptr;
	filename.clear();
	return true;
	}

bool file::write( const uint8_t *data, const size_t length, size_t *bytes_written )
    {
    if ( !file_handle )
        return false;
    size_t offset = 0;
    while ( offset < length )
        {
        size_t result = fwrite( data + offset, 1, length - offset, file_handle );
        if ( result == 0 )
            return false;
        if ( bytes_written )
            *bytes_written += result;
        offset += result;
        }
    return true;
    }

bool file::read( const uint8_t *data, const size_t length, size_t *bytes_read )
    {
    if ( !file_handle )
        return false;
    size_t offset = 0;
    while ( offset < length )
        {
        size_t result = fread((void*) (data + offset), 1, length - offset, file_handle );
        if ( result == 0 )
            return false;
        if ( bytes_read )
            *bytes_read += result;
        offset += result;
        }
    return true;
    }

bool file::seek( const off_t offset, const seek_start_point start_point )
    {
    if ( !file_handle )
        return false;
    static long translations[3] = { SEEK_SET, SEEK_END, SEEK_CUR };
    int translated_seek_point=translations[(int)start_point];
    fseek( file_handle, offset, translated_seek_point );
    return true;
    }

bool file::tell( uint64_t &offset )
    {
    if ( !file_handle )
        return false;
    offset = ftell( file_handle );
    return true;
    }
}
}