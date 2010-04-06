/*
 * Base.hpp
 *
 */
#ifndef INCLUDE_LOGGER_APPENDERS_BASE_HPP_FILE
#define INCLUDE_LOGGER_APPENDERS_BASE_HPP_FILE

#include <boost/shared_ptr.hpp>
#include <cassert>

#include "Base/Threads/Mutex.hpp"
#include "Base/Threads/Lock.hpp"


namespace Logger
{
namespace Appenders
{

/** \brief common interface for all appenders.
 */
class Base
{
public:
  /** \brief ensure polymorphic destruction.
   */
  virtual ~Base(void)
  {
  }

  /** \brief appends given string (calls appender) implenetation.
   *  \param str message to be appended.
   *  \note call is thread-safe.
   */
  void append(const std::string &str)
  {
    ::Base::Threads::Lock lock(mutex_);
    appendImpl(str);
  }

  /** \brief returns name of this appenders type.
   *  \return compile-time known string representing appender's type.
   */
  virtual const char *getTypeName(void) const
  {
    const char *name=getTypeNameImpl();
    assert(name!=NULL);
    return name;
  }

private:
  /** \brief template-method pattern - user implements this call.
   *  \param str message to be appended.
   */
  virtual void appendImpl(const std::string &str) = 0;

  /** \brief retruns name of a given type.
   *  \return type name for a given appender.
   *  \note string must be known compile-time to prevent races and other
   *        artefacts associated with 'const char*'.
   */
  virtual const char *getTypeNameImpl(void) const = 0;

  ::Base::Threads::Mutex mutex_;
}; // class Base


/** \brief shared pointer for base class.
 */
typedef boost::shared_ptr<Base> BasePtr;

} // namespace Appenders
} // namespace Logger

#endif
