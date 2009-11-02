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

File::File(const std::string &path):
  Stream<File>(out_)
{
  // this is a bit ctricky, since first opeartion is passing reference to an
  // object in derived class to its base and then object is initialized.
  // though strange it is fine, since object is not used in a c-tor of base
  // class and reference will not change anyway (object is part of a class).

  out_.open(path.c_str(), ios_base::out | ios_base::app | ios_base::binary);
  assert( out_.is_open() );
}

} // namespace Appenders
} // namespace Logger
