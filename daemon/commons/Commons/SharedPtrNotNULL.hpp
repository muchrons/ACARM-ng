/*
 * SharedPtrNotNull.hpp
 *
 */
#ifndef INCLUDE_COMMONS_SHAREDPTRNOTNULL_HPP_FILE
#define INCLUDE_COMMONS_SHAREDPTRNOTNULL_HPP_FILE

/* public header */

#include <boost/shared_ptr.hpp>

#include "Commons/ExceptionUnexpectedNULL.hpp"

namespace Commons
{

// TODO

template<typename T>
class SharedPtrNotNULL
{
private:
  typedef boost::shared_ptr<T> PtrT;
public:
  SharedPtrNotNULL(T *t):
    ptr_(t)
  {
    ensure();
  }

  const T &operator*(void) const
  {
    return *ptr_;
  }

private:
  void ensure(void)
  {
    ensure( ptr_.get() );
  }

  void ensure(const T *t)
  {
    if(t==NULL)
      throw ExceptionUnexpectedNULL(__FILE__);
  }

  PtrT ptr_;
}; // struct SharedPtrNotNULL

} // namespace Commons

#endif
