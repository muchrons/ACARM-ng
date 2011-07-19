/*
 * readTextFile.cpp
 *
 */
#include "Commons/Filesystem/readTextFile.hpp"

using namespace std;


namespace Commons
{
namespace Filesystem
{

boost::shared_array<char> readTextFile(const boost::filesystem::path &p)
{
  SharedPtrNotNULL<std::fstream>  fs=openFile(p, Mode::READ);
  std::istream                   &is=*fs;

  // get file size
  is.seekg(0, ios::end);
  const size_t size=is.tellg();
  is.seekg(0, ios::beg);
  // prepare output memory
  boost::shared_array<char> buf(new char[size+1]);

  // read all data at once
  is.read(buf.get(), size);
  buf[size]=0;                  // NUL-terminate string

  // return the result
  return buf;
} // readTextFile()

} // namespace Filesystem
} // namespace Commons
