/*
 * File.cpp
 *
 */
#include <cassert>

#include "Logger/Appenders/File.hpp"

using namespace std;

namespace Logger
{
namespace Appenders
{

File::File(const boost::filesystem::path &path):
  Stream<File>(out_),
  path_(path)
{
  // this is a bit ctricky, since first opeartion is passing reference to an
  // object in derived class to its base and then object is initialized.
  // though strange it is fine, since object is not used in a c-tor of base
  // class and reference will not change anyway (object is part of a class).

  reinit();
}

void File::reinitImpl(void)
{
  // close file first, if needed
  if( out_.is_open() )
    out_.close();

  // open file and check if it succedded.
  out_.open(path_.string().c_str(), ios_base::out | ios_base::app | ios_base::binary);
  if( !out_.is_open() )
    throw ExceptionFileAccessError(SYSTEM_SAVE_LOCATION, path_);
  // sanity check
  assert( out_.is_open() );
}

} // namespace Appenders
} // namespace Logger
