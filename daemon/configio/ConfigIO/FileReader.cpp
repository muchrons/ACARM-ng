/*
 * FileReader.cpp
 *
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "System/DiskFile.hpp"
#include "ConfigIO/FileReader.hpp"

using namespace std;
using namespace System;

namespace ConfigIO
{

FileReader::FileReader(const boost::filesystem::path &path)
{
  DiskFile df(path.string(), O_RDONLY|O_LARGEFILE);

  // get file size
  {
    struct stat fs;
    if( fstat(df.get(), &fs)!=0 )
      throw ExceptionFileAccessError(SYSTEM_SAVE_LOCATION, path);
    size_=fs.st_size;
  }

  // read file's content
  {
    // allocate proper amount of memory
    data_.reset(new Byte[size_+1]);     // last byte is '\0' - for text files.
    // read file's content
    const int size=size_;               // suppresses signed/unsigned warning.
    if( read(df.get(), data_.get(), size_)!=size )
      throw ExceptionFileAccessError(SYSTEM_SAVE_LOCATION, path);
    // mark last byte as terminating zero, in case of text files, so that
    // it could be read as regular string.
    data_[size_]='\0';
  }
}

} // namespace ConfigIO
