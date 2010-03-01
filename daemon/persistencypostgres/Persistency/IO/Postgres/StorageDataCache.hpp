/*
 * StorageDataCache.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_STORAGEDATACACHE_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_STORAGEDATACACHE_HPP_FILE

#include <map>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <cassert>

#include "Base/Threads/Mutex.hpp"
#include "Base/Threads/Lock.hpp"
#include "Persistency/IO/Postgres/DataBaseID.hpp"
#include "Persistency/IO/Postgres/Exception.hpp"

namespace Persistency
{
namespace IO
{
namespace Postgres
{
/** \brief creates object's instance to ID mapping.
 */
template<typename T>
class StorageDataCache: private boost::noncopyable
{
public:
  /** \brief type of pointer to a type. */
  typedef T*                   TPtr;
  /** \brief type of shared pointer to a type. */
  typedef boost::shared_ptr<T> TSharedPtr;
  /** \brief type of weak pointer to a type. */
  typedef boost::weak_ptr<T>   TWeakPtr;

private:
  struct EntryID
  {
    EntryID(TSharedPtr ptr, DataBaseID id):
      ptr_(ptr),
      id_(id)
    {
    }

    TWeakPtr   ptr_;
    DataBaseID id_;
  }; // struct EntryID

  typedef std::map<TPtr, EntryID> ObjectIDMapping;  // std::map is used instead of the std::set
                                                    // since weak_ptr<> may become NULL and this
                                                    // would cause problems with finding with
                                                    // indexes.
public:
  /** \brief exception thrown when entry does not exist.
   */
  struct ExceptionNoSuchEntry: public Exception
  {
    /** \brief c-tor with proper error message.
     *  \param where place where exception has been raised.
     */
    explicit ExceptionNoSuchEntry(const Location &where):
      Exception(where, "entry to be read does not exist")
    {
    }
  }; // struct ExceptionEntryAlreadyExist

  /** \brief gets ID for a given object.
   *  \param ptr pointer to object to check.
   *  \return id if given object in data base.
   */
  DataBaseID get(TSharedPtr ptr)
  {
    Base::Threads::Lock lock(mutex_);
    typename ObjectIDMapping::iterator it=oidm_.find( ptr.get() );
    if( it==oidm_.end() || ptr.get()==NULL )
      throw ExceptionNoSuchEntry(SYSTEM_SAVE_LOCATION);
    // both these asserts are known to be true, since we have object's
    // instance as a parameter, so pointer either is already deallocated
    // or is still valid, and so assertion's hold.
    assert( it->second.ptr_.lock().get()!=NULL      );
    assert( it->second.ptr_.lock().get()==ptr.get() );
    return it->second.id_;
  }

  /** \brief exception throw when trying to add dupliaceted entry.
   */
  struct ExceptionEntryAlreadyExist: public Exception
  {
    /** \brief c-tor with proper error message.
     *  \param where place where exception has been rised.
     */
    explicit ExceptionEntryAlreadyExist(const Location &where):
      Exception(where, "entry to be added already exist")
    {
    }
  }; // struct ExceptionEntryAlreadyExist

  /** \brief adds new mapping to collection.
   *  \param ptr pointer to be added (as a key).
   *  \param id  data base ID for this object.
   */
  void add(TSharedPtr ptr, DataBaseID id)
  {
    const EntryID                      tmp(ptr, id);
    Base::Threads::Lock                lock(mutex_);
    typename ObjectIDMapping::iterator it=oidm_.find( ptr.get() );
    // check for duplicates
    if( it!=oidm_.end() )
    {
      if( it->second.ptr_.lock().get()!=NULL )
        throw ExceptionEntryAlreadyExist(SYSTEM_SAVE_LOCATION);
      // if we're here it means we have dead entrym with dongling pointer
      // to re-allocated place in memory
      it->second=tmp;
    }
    else
    {
      // insert new entry to collection
      oidm_.insert( typename ObjectIDMapping::value_type( ptr.get(), tmp ) );
    }
    // however we got here (overwriting dongling pointer, or inserting totaly
    // new entry) following conditions must hold (note: since we're still in
    // critical section there is no possibility of race conditions):
    assert( oidm_.find( ptr.get() )!=oidm_.end() );
    assert( oidm_.find( ptr.get() )->second.ptr_.lock().get()!=NULL );
    assert( oidm_.find( ptr.get() )->second.ptr_.lock().get()==ptr.get() );
    assert( oidm_.find( ptr.get() )->second.id_==id);
  }

  /** \brief performs collection cleanup of object to used anymore.
   */
  void prune(void)
  {
    Base::Threads::Lock lock(mutex_);
    typename ObjectIDMapping::iterator it=oidm_.begin();
    while( it!=oidm_.end() )
    {
      // is anything to do for this element?
      if( it->second.ptr_.lock().get()!=NULL )
      {
        ++it;
        continue;
      }
      // remove this element
      typename ObjectIDMapping::iterator tmp=it;
      ++it;
      oidm_.erase(tmp);
    } // while(!end)
  }

  /** \brief gets collection size (i.e. number of elements).
   *  \return total number of elements in collection.
   *  \note this call is mostely for stats and logs, since information returned
   *        may be invalidated straight away (internal lock is released).
   */
  size_t size(void) const
  {
    Base::Threads::Lock lock(mutex_);
    return oidm_.size();
  }

private:
  mutable Base::Threads::Mutex mutex_;
  ObjectIDMapping              oidm_;
}; // class StorageDataCache

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
