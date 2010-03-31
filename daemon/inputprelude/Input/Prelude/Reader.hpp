/*
 * Reader.hpp
 *
 */
#ifndef INCLUDE_INPUT_PRELUDE_READER_HPP_FILE
#define INCLUDE_INPUT_PRELUDE_READER_HPP_FILE

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

#include "Input/Reader.hpp"
#include "Input/Prelude/Client.hpp"
#include "Input/Prelude/LogCallback.hpp"

// TODO: implement
// TODO: test

namespace Input
{
namespace Prelude
{

/** \brief generic input interface class.
 */
class Reader: public Input::Reader
{
public:
  /** \brief create prelud reader.
   */
  Reader(const std::string profile, const std::string config);
  /** \brief reads data from input from prelude-manager.
   *  \param timeout milliseconds to wait before timeout occures.
   *  \return pointer to newly allocated strucutre or NULL on timeout.
   */
  virtual DataPtr read(unsigned int timeout);

private:
  LogCallback               preludeLogger_;
  std::string               preludeProfile_;
  boost::scoped_ptr<Client> client_;
}; // class Reader

} // namespace Prelude
} // namespace Input

#endif
