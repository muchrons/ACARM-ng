/*
 * CmdLineArgs.cpp
 *
 */
#include <cstring>
#include <cassert>

#include "Commons/CmdLineArgs.hpp"


namespace Commons
{

CmdLineArgs::ExceptionInvalidArgument::ExceptionInvalidArgument(const Location &where, const char *what):
  Exception(where, what)
{
}


CmdLineArgs::CmdLineArgs(const int argc, char const * const * const argv):
  argc_(argc)
{
  // sanity check
  if(argc_<1)
    throw ExceptionInvalidArgument(SYSTEM_SAVE_LOCATION, "arguments count cannot be less than 1");
  if(argv==NULL)
    throw ExceptionInvalidArgument(SYSTEM_SAVE_LOCATION, "arguments array cannot be NULL");

  // prepare output data structures
  argvElems_.reserve(argc_);
  argv_.reset(new char*[argc_]);
  assert( argv_.get()!=NULL );

  // copy all of the arguments
  for(int i=0; i<argc_; ++i)
  {
    // sanity check
    if(argv[i]==NULL)
      throw ExceptionInvalidArgument(SYSTEM_SAVE_LOCATION, "argument cannot be NULL");
    // copy string
    const size_t              len=strlen(argv[i]);
    boost::shared_array<char> tmp(new char[len+1]);
    memcpy(tmp.get(), argv[i], len+1);
    assert( strlen(tmp.get())==len );   // memcpy() is faster and we know length any way
    // put it into final locations
    argvElems_.push_back(tmp);
    argv_[i]=tmp.get();
  }
}


char **CmdLineArgs::argv(void) const
{
  assert( argv_.get()!=NULL );
  return argv_.get();
}


int CmdLineArgs::argc(void) const
{
  assert(argc_>=1);
  return argc_;
}

} // namespace Commons
