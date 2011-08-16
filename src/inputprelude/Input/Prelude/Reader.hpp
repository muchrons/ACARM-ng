/*
 * Reader.hpp
 *
 */
#ifndef INCLUDE_INPUT_PRELUDE_READER_HPP_FILE
#define INCLUDE_INPUT_PRELUDE_READER_HPP_FILE

#include <boost/scoped_ptr.hpp>

#include "Input/Reader.hpp"
#include "Input/Prelude/Client.hpp"

namespace Input
{
namespace Prelude
{

/** \brief generic input interface class.
 */
class Reader: public Input::Reader
{
public:
  /** \brief create prelude reader.
   * \param profile A "profile" for prelude library
   * \param name Name of the instance
   * \param config A path to a prelude config
   * \param heartbeatTimeout timeout for prelude heartbeats
   */
  Reader(const std::string                     &profile,
         const Core::Types::Proc::InstanceName &name,
         const std::string                     &config,
         unsigned int                           heartbeatTimeout);

  /** \brief reads data from input from prelude-manager.
   *  \param bf      backend facade to use for processing.
   *  \param timeout milliseconds to wait before timeout occures.
   *  \return pointer to newly allocated strucutre or NULL on timeout.
   */
  virtual DataPtr read(BackendFacade &bf, unsigned int timeout);

private:
  boost::scoped_ptr<Client> client_;
  const unsigned int        heartbeatTimeout_;
}; // class Reader

} // namespace Prelude
} // namespace Input

#endif
