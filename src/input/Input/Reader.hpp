/*
 * Reader.hpp
 *
 */
#ifndef INCLUDE_INPUT_READER_HPP_FILE
#define INCLUDE_INPUT_READER_HPP_FILE

/* public header */

#include <boost/noncopyable.hpp>

#include "Commons/SharedPtrNotNULL.hpp"
#include "Logger/Logger.hpp"
#include "Persistency/Alert.hpp"
#include "Input/BackendFacade.hpp"

namespace Input
{

/** \brief generic input interface class.
 */
class Reader: private boost::noncopyable
{
public:
  /** \brief smart pointer to data returned from read() call.
   */
  typedef Persistency::AlertPtr DataPtr;

  /** \brief start virtual d-tor hierarchy.
   */
  virtual ~Reader(void);

  /** \brief gets type of this reader.
   *  \return reader's type.
   */
  const std::string &getType(void) const
  {
    return type_;
  }
  /** \brief gets name of this reader.
   *  \return reader's name.
   */
  const std::string &getName(void) const
  {
    return name_;
  }

  /** \brief reads data from input or returns if timeouted.
   *  \param bf      backend faced to be used.
   *  \param timeout milliseconds to wait before timeout occures. timeout=0
   *                 means infinity.
   *  \return pointer to newly allocated strucutre or NULL on timeout.
   *  \note method in non-const on purpose, to allow internal buffering and
   *        doing actual reading in separate thread.
   */
  virtual DataPtr read(BackendFacade &bf, unsigned int timeout=0) = 0;

protected:
  /** \brief create base objects.
   *  \param type type of this reader.
   *  \param name name of this reader.
   */
  Reader(const std::string &type, const std::string &name);

  const Logger::Node log_;  ///< logger node to log messages to.

private:
  const std::string type_;
  const std::string name_;
}; // class Reader


/** \brief non-NULL pointer to reader. */
typedef Commons::SharedPtrNotNULL<Reader> ReaderPtrNN;

} // namespace Input

#endif
