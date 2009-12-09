/*
 * FileUnlinker.t.hpp
 *
 */
#ifndef INCLUDE_APPENDERS_FILEUNLINKER_HPP_FILE
#define INCLUDE_APPENDERS_FILEUNLINKER_HPP_FILE

#include <string>
#include <unistd.h>

namespace
{

//
// helper class that gets file name in c-tor and unlinks it in
// d-tor. it is useful in tests, when one might expect that exception
// will break usual call sequence, but cleanup is a must.
//
class FileUnlinker
{
public:
  FileUnlinker(const std::string &path):
    path_(path)
  {
  }

  ~FileUnlinker(void)
  {
    unlink( path_.c_str() );
  }

private:
  const std::string path_;
}; // class FileUnlinker

} // unnamed namespace

#endif
