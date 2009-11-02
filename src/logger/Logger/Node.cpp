/*
 * Node.cpp
 *
 */
#include <sstream>
#include <sys/timeb.h>
#include <cassert>

#include "Logger/Node.hpp"
#include "Logger/ConfigSet.hpp"
#include "Logger/Formatter.hpp"

using namespace std;


namespace Logger
{

Node::Node(const NodeName &nn):
  nn_(nn),
  nc_( ConfigSet::getConfig(nn_) )
{
  assert( nc_.get()!=NULL );
}

Node::~Node(void)
{
  // thought this looks strange it allows proper generation of NodeConfPtr
  // destructor, that calls NodeConf's one, via 'delete' from share_ptr<>.
}

void Node::log(Priority      pri,
               const char   *file,
               const char   *call,
               unsigned int  line,
               const char   *msg) const
{
  // get current time-stamp
  timeb ts;
  ftime(&ts);

  // format string
  stringstream ss;
  nc_->getFormatter().format(ss, ts, nn_, pri, file, call, line, msg);
  // append it to configured appender
  nc_->getAppender()->append( ss.str() );
}

} // namespace Logger
