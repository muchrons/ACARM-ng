/*
 * Reader.hpp
 *
 */
#ifndef INCLUDE_INPUT_READER_HPP_FILE
#define INCLUDE_INPUT_READER_HPP_FILE

/* public header */

#include <memory>
#include <boost/noncopyable.hpp>

namespace Input
{

/** \brief generic input interface class.
 */
class Reader: private boost::noncopyable
{
public:
  // TODO: this declaration should use polymorphic type from Persistency,
  //       instead of 'double' (it is just for tests).

  /** \brief smart pointer to data returned from read() call.
   */
  typedef std::auto_ptr<double> DataPtr;

  /** \brief start virtual d-tor hierarchy.
   */
  virtual ~Reader(void)
  {
  }

  /** \brief reads data from input or returns if timeouted.
   *  \param timeout milliseconds to wait before timeout occures. timeout=0
   *                 means infinity.
   *  \return pointer to newly allocated strucutre or NULL on timeout.
   *  \note method in non-const on purpose, to allow internal buffering and
   *        doing actual reading in separate thread.
   */
  virtual DataPtr read(unsigned int timeout=0) = 0;

}; // class Reader

} // namespace Input

#endif
