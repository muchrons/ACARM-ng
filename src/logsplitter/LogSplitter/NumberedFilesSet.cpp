/*
 * NumberedFilesSet.cpp
 *
 */
#include <string>
#include <sstream>
#include <stdexcept>

#include "LogSplitter/NumberedFilesSet.hpp"

using namespace std;


namespace LogSplitter
{

NumberedFilesSet::NumberedFilesSet(const std::string &prefix, const unsigned int bytes):
  prefix_(prefix),
  bytes_(bytes)
{
}

NumberedFilesSet::FileStreamPtr NumberedFilesSet::get(unsigned int n)
{
  if(out_.size()<n+1)
    out_.resize(n+1);

  if(out_[n].get()==NULL)
  {
    const string  name=makeFileName(n);
    FileStreamPtr ptr(new OutFile(name, bytes_));
    out_[n].swap(ptr);
  }

  assert(out_.at(n).get()!=NULL);
  return out_[n];
}

void NumberedFilesSet::flush(void)
{
  for(Outputs::iterator it=out_.begin(); it!=out_.end(); ++it)
    if(it->get()!=NULL)
      (*it)->flush();
}

std::string NumberedFilesSet::makeFileName(unsigned int n)
{
  stringstream ss;
  ss<<prefix_<<n<<".log";
  return ss.str();
}

} // namespace LogSplitter
