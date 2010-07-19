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

  /** \brief reinits the appender (ex. closes and opens output once again).
   *  \note call is thread-safe.
   */
  void reinit(void)
  {
    ::Base::Threads::Lock lock(mutex_);
    reinitAlreadyLocked();
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

protected:
  /** \brief runs reinitImpl() without closing mutex.
   *
   *  this call is mostly needed when user wants to call reinit from appendImpl(),
   *  since reinit() uses the same mutex and it would cause hang. use with care!
   */
  void reinitAlreadyLocked(void)
  {
    assert( ::Base::Threads::Mutex::scoped_try_lock(mutex_).owns_lock()==false &&
            "mutex is NOT locked - you probably wanted to run reinit() here" );
    reinitImpl();
  }

private:
  /** \brief template-method pattern - user implements this call.
   *  \param str message to be appended.
   */
  virtual void appendImpl(const std::string &str) = 0;
  /** \brief template-method pattern - user implements this call.
   */
  virtual void reinitImpl(void) = 0;

  /** \brief retruns name of a given type.
   *  \return type name for a given appender.
   *  \note string must be known compile-time to prevent races and other
   *        artefacts associated with 'const char*'.
   */
  virtual const char *getTypeNameImpl(void) const = 0;

  mutable ::Base::Threads::Mutex mutex_;
}; // class Base


/** \brief shared pointer for base class.
 */
typedef boost::shared_ptr<Base> BasePtr;

} // namespace Appenders
} // namespace Logger

#endif
