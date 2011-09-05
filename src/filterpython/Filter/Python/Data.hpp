/*
 * Data.hpp
 *
 */
#ifndef INCLUDE_FILTER_PYTHON_DATA_HPP_FILE
#define INCLUDE_FILTER_PYTHON_DATA_HPP_FILE

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

namespace Filter
{
namespace Python
{

/** \brief base class for python data implementation).
 *
 *  this class does not have C++ methods since, by design, it is
 *  generic data container to be used in python only. C++ must
 *  only be able to destroy it.
 *
 *  this class should be derived from and implemented in Python.
 *  then it can be used as a data associated with given node.
 */
struct Data: private boost::noncopyable
{
  /** \brief ensure polymorphic destruction.
   */
  virtual ~Data(void);
}; // struct Data


/** \brief smart pointer to allow secure, polymorphic destruction. */
typedef boost::shared_ptr<Data> DataPtr;

} // namespace Python
} // namespace Filter

#endif
