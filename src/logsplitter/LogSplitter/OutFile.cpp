/*
 * OutFile.cpp
 *
 */
#include "LogSplitter/OutFile.hpp"

using namespace std;


namespace LogSplitter
{

OutFile::OutFile(const std::string &file)
{
  file_.open(file.c_str(), ios_base::binary|ios_base::trunc|ios_base::out);
  if( !file_.is_open() )
    throw runtime_error("unable to open file: "+file);
}

void OutFile::flush(void)
{
  // TODO
}

} // namespace LogSplitter
