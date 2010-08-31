/*
 * Strategy.cpp
 *
 */
#include <fstream>
#include <cassert>

#include "Trigger/Compose/Full.hpp"
#include "Trigger/File/Strategy.hpp"

using namespace std;

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

void Strategy::triggerImpl(const Node &n)
{
  // prepare data
  stringstream ss;
  Compose::Full::append(ss, n);
  // open output
  const string path=createOutputPath();
  LOGMSG_DEBUG_S(log_)<<"output file is: "<<path;
  ofstream     file( path.c_str() );
  if( !file.is_open() )
    throw ExceptionCannotOpenFile(SYSTEM_SAVE_LOCATION, path, "couldn't open output file");
  // write data
  file << ss.str();
}

std::string Strategy::createOutputPath(void)
{
  // check if something has been already written this second
  const time_t now=time(NULL);
  if(now!=lastWrite_)
  {
    lastWrite_=now;
    lastIndex_=0;
  }
  // TODO: this should be fixed - it should use mktemp() call with proper parameter
  // compose final path
  stringstream path;
  path << outdir_ << "/" << now << "_" << lastIndex_ << ".txt";
  // increment count
  ++lastIndex_;
  // return final response
  return path.str();
}

} // namespace File
} // namespace Trigger
