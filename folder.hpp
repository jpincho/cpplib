#pragma once
#include "string.hpp"
#include "array.hpp"

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
		bool set( const char *foldername );
		bool list( array <string> &filelist );
	};
}
}