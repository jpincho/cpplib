#pragma once
#include "../string.hpp"
#include "../array.hpp"

namespace cpplib
{
namespace filesystem
{
class folder
	{
	private:
		string foldername;
	public:
		folder( void );
		~folder( void );
		bool set( const char *new_foldername );
		bool list( array <string> &entries, const bool recursive = false );
	};
}
}