/*
 * Reader.hpp
 *
 */
#ifndef INCLUDE_INPUT_PRELUDE_READER_HPP_FILE
#define INCLUDE_INPUT_PRELUDE_READER_HPP_FILE

/* public header */

#include <memory>
#include <boost/noncopyable.hpp>
#include <PreludePP/idmef.hpp>
#include <PreludePP/prelude-client.hpp>

#include "Input/Reader.hpp"
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
  Reader(const std::string profile);
public:
  /** \brief reads data from input from prelude-manager.
   *  \param timeout milliseconds to wait before timeout occures.
   *  \return pointer to newly allocated strucutre or NULL on timeout.
   */
  virtual DataPtr read(unsigned int timeout);

private:
  LogCallback preludeLogger_;
  std::string prelude_profile;
  std::auto_ptr< ::Prelude::Client > client;
}; // class Reader

} // namespace Prelude
} // namespace Input

#endif
