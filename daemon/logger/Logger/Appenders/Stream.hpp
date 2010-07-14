/*
 * Stream.hpp
 *
 */
#ifndef INCLUDE_LOGGER_APPENDERS_STREAM_HPP_FILE
#define INCLUDE_LOGGER_APPENDERS_STREAM_HPP_FILE

#include "Logger/Appenders/Base.hpp"


namespace Logger
{
namespace Appenders
{

/** \brief stream logger - messages are written to a stream.
 */
template<typename CRTP>
class Stream: public Base
{
protected:
  /** \brief creates stream appender from a given stream.
   *  \param os output stream to be used.
   */
  explicit Stream(std::ostream &os):
    os_(os)
  {
    // NOTE: os_ might not be initialized at this point yet!
  }

private:
  virtual void appendImpl(const std::string &str)
  {
    if( os_.bad() )
    {
      os_.clear();              // clear error flags
      reinitAlreadyLocked();    // try reinitializing stream
    }
    os_<<str<<std::endl;
  }

  virtual const char *getTypeNameImpl(void) const
  {
    return CRTP::getThisTypeName();
  }

  std::ostream &os_;
}; // class Console

} // namespace Appenders
} // namespace Logger

#endif
