/*
 * Strategy.cpp
 *
 */
#include <fstream>
#include <cassert>

#include "Commons/Filesystem/createFile.hpp"
#include "Trigger/Compose/Full.hpp"
#include "Trigger/File/Strategy.hpp"

using namespace std;
using Commons::SharedPtrNotNULL;
using Commons::Filesystem::createFile;

namespace Trigger
{
namespace File
{

Strategy::Strategy(const std::string &name, const Config &cfg):
  Trigger::Simple::Strategy("file", name, cfg.getThresholdConfig() ),
  outdir_( cfg.getOutputDirectory() ),
  lastWrite_(0),
  lastIndex_(0)
{
}

void Strategy::triggerImpl(const ConstNode &n)
{
  // prepare data
  stringstream ss;
  Compose::Full::append(ss, n);
  // open output
  const boost::filesystem::path path=createOutputPath();
  LOGMSG_DEBUG_S(log_)<<"output file is: "<<path;
  // create output file
  Commons::SharedPtrNotNULL<fstream> file=Commons::Filesystem::createFile(path);
  assert( file->is_open() );
  // write data
  *file << ss.str();
}

boost::filesystem::path Strategy::createOutputPath(void)
{
  // check if something has been already written this second
  const time_t now=time(NULL);
  if(now!=lastWrite_)
  {
    lastWrite_=now;
    lastIndex_=0;
  }
  // compose final path
  stringstream fileName;
  fileName << now << "_" << lastIndex_ << ".txt";
  // increment count
  ++lastIndex_;
  // return final response
  return outdir_ / fileName.str();
}

} // namespace File
} // namespace Trigger
