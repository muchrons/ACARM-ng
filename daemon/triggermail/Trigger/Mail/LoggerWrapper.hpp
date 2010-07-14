/*
 * LoggerWrapper.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_MAIL_LOGGERWRAPPER_HPP_FILE
#define INCLUDE_TRIGGER_MAIL_LOGGERWRAPPER_HPP_FILE

#include <boost/noncopyable.hpp>

#include "Logger/Node.hpp"

namespace Trigger
{
namespace Mail
{

/** \brief wrapper for initialization of logging from etpan library.
 *
 *  this class is not copyable, but can be instanciated many times. always
 *  last instance's d-tor will do the clean-up.
 */
struct LoggerWrapper: private boost::noncopyable
{
  /** \brief register logger handle.
   */
  LoggerWrapper(void);
  /** \brief unregister logger handle.
   */
  ~LoggerWrapper(void);
}; // struct LoggerWrapper

} // namespace Mail
} // namespace Trigger

#endif
