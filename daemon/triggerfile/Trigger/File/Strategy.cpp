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

Strategy::Strategy(const Config &cfg):
  Trigger::Simple::Strategy("file", cfg.getThresholdConfig() ),
  outdir_( cfg.getOutputDirectory() ),
  lastWrite_(0),
  lastIndex_(0)
{
}

void Strategy::trigger(const Node &n)
{
  // prepare data
  stringstream ss;
  Compose::Full::append(ss, n);
  // open output
  const string path=createOutputPath();
  ofstream     file( path.c_str() );
  if( !file.is_open() )
    throw ExceptionCantOpenFile(SYSTEM_SAVE_LOCATION, path);
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
