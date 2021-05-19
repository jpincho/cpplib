#include "folder.hpp"

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

bool folder::set( const char *foldername )
	{
	this->foldername = foldername;
	return true;
	}

bool folder::list( array<string> &filelist )
	{

	}
}
}