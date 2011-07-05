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

NumberedFilesSet::NumberedFilesSet(const std::string &prefix):
  prefix_(prefix)
{
}

NumberedFilesSet::FileStreamPtr NumberedFilesSet::get(unsigned int n)
{
  if(out_.size()<n+1)
    out_.resize(n+1);

  if( out_[n].get()==NULL )
  {
    FileStreamPtr ptr(new std::ofstream);
    const string name=makeFileName(n);
    ptr->open(name.c_str(), ios_base::binary|ios_base::trunc|ios_base::out);
    if( !ptr->is_open() )
      throw runtime_error("unable to open file: "+name);
    out_[n].swap(ptr);
  }

  assert(out_.at(n).get()!=NULL);
  return out_[n];
}

std::string NumberedFilesSet::makeFileName(unsigned int n)
{
  stringstream ss;
  ss<<prefix_<<n<<".log";
  return ss.str();
}

} // namespace LogSplitter
