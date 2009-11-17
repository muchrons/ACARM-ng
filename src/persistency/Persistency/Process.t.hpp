/*
 * Process.t.cpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_PROCESS_T_HPP_FILE
#define INCLUDE_PERSISTENCY_PROCESS_T_HPP_FILE

#include "Persistency/Process.hpp"

using namespace std;
using namespace Persistency;

namespace
{

struct ProcessTestImpl: public Process
{
  ProcessTestImpl(const char *path, const char *name, const MD5Sum *md5):
    Process(path, name, md5)
  {
  }
}; // struct TestImpl

} // unnamed namespace

#endif
