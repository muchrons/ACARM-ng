/*
 * OutFile.cpp
 *
 */
#include "LogSplitter/OutFile.hpp"

using namespace std;


namespace LogSplitter
{

OutFile::OutFile(const std::string &file, const unsigned int bytes):
  buf_(new stringstream),
  bytes_(bytes)
{
  file_.open(file.c_str(), ios_base::binary|ios_base::trunc|ios_base::out);
  if( !file_.is_open() )
    throw runtime_error("unable to open file: "+file);
}

void OutFile::flush(void)
{
  // empty call?
  if(buf_->tellp()==0)
    return;

  // flush buffers
  boost::scoped_ptr<stringstream> tmp(new stringstream);
  assert(buf_.get()!=NULL);
  file_<<buf_->str();
  buf_.swap(tmp);
  assert(buf_->tellp()==0);
}

} // namespace LogSplitter
