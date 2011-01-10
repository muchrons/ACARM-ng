/*
 * WaitingLockData.hpp
 *
 */
#include <boost/noncopyable.hpp>

#include "Base/Threads/Mutex.hpp"
#include "Base/Threads/ReadWriteMutex.hpp"
#include "Base/Threads/ReadTryLock.hpp"
#include "Base/Threads/Conditional.hpp"
#include "Persistency/GraphNodePtr.hpp"
#include "Persistency/detail/NonCyclicAdder.hpp"

namespace Persistency
{
namespace detail
{

/** \brief holder for pointer to blocking item.
 */
class WaitingLockData
{
public:
  /** \brief created object reset's WaitingLockData pointer uppon release.
   *
   *  this helper sets pointer to NULL, to mark that node does not wait for
   *  anything, while not in operational mode.
   */
  class ResetOnRelease: private boost::noncopyable
  {
  public:
    /** \brief create instance.
     *  \param wld object to reset when exiting.
     */
    explicit ResetOnRelease(WaitingLockData &wld);
    /** \brief reest saved instance.
     */
    ~ResetOnRelease(void);

  private:
    WaitingLockData &wld_;
  }; // class ResetOnRelease


  /** \brief created object signals WaitingLockData users uppon release.
   *
   *  this helper is to be used to signal that lock is not used any more.
   */
  class SignalOnRelease: private boost::noncopyable
  {
  public:
    /** \brief create instance.
     *  \param wld object to signal when exiting.
     */
    explicit SignalOnRelease(WaitingLockData &wld);
    /** \brief signal saved instance.
     */
    ~SignalOnRelease(void);

  private:
    WaitingLockData &wld_;
  }; // class SignalOnRelease


  /** \brief create instance.
   */
  WaitingLockData(void);
  /** \brief clean up object.
   */
  ~WaitingLockData(void);

  /** \brief set new poitern value.
   *  \param ptr pointer ot be set.
   */
  void setPtr(GraphNodePtr ptr);
  /** \brief get curren pointer's value.
   *  \return value of the pointer held inside.
   */
  GraphNodePtr getPtr(void) const;
  /** \brief get value, or wait for proper condition when locking fails.
   *  \param ptr pointer to compare current one with - if they differ, new value is returned.
   *             otherwise call waits for event.
   *  \param rtl try-lock to be try-locked when signaled.
   *  \return currently saved pointer's value.
   */
  GraphNodePtr getWhenDifferOrLocked(GraphNodePtr ptr, Base::Threads::ReadTryLock &rtl);
  /** \brief notify other on change.
   *
   *  notification must be made when write-lock is released on object too. see
   *  SignalOnRelease helper class for more details.
   */
  void signalAll(void);

private:
  void setPtrImpl(GraphNodePtr ptr);

  mutable Base::Threads::Mutex       mutex_;
  mutable Base::Threads::Conditional cond_;
  GraphNodePtr                       ptr_;  // node we're waiting for.
}; // class WaitingLockData

} // namespace detail
} // namespace Persistency
