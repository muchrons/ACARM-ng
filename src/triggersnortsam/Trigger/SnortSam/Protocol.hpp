/*
 * Protocol.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_SNORTSAM_PROTOCOL_HPP_FILE
#define INCLUDE_TRIGGER_SNORTSAM_PROTOCOL_HPP_FILE

#include <string>
#include <boost/noncopyable.hpp>

#include "Logger/Logger.hpp"
#include "Commons/SharedPtrNotNULL.hpp"
#include "Trigger/SnortSam/Config.hpp"
#include "Trigger/SnortSam/Exception.hpp"


namespace Trigger
{
namespace SnortSam
{
/** \brief base class for the protocol implementations.
 */
class Protocol: private boost::noncopyable
{
public:
  /** \brief creates base object.
   */
  Protocol(void);
  /** \brief ensures polymorphic destruction.
   */
  virtual ~Protocol(void);

  /** \brief blocks given host->host communication.
   *  \param from source host.
   *  \param to   destination host.
   */
  void block(const Config::IP &from, const Config::IP &to);
  /** \brief deinitializes protocol.
   */
  void deinit(void);

protected:
  /** \brief gets protocol version.
   *  \return string with version information.
   */
  const std::string &getProtocolVersion(void);
  /** \brief returns connection status.
   *  \return true if protocl is (logically) connected with remote host, false otherwise.
   */
  bool isConnected(void);
  /** \brief establishes (logical) connection to remote host.
   */
  void init(void);

  const Logger::Node log_;      ///< logger node to use for loggin messages

private:
  virtual const std::string &getProtocolVersionImpl(void) const = 0;
  virtual bool isConnectedImpl(void) const = 0;
  virtual void initImpl(void) = 0;
  virtual void deinitImpl(void) = 0;
  virtual void blockImpl(const Config::IP &from, const Config::IP &to) = 0;
}; // class Protocol


/** \brief pointer to the protocol's implementation. */
typedef Commons::SharedPtrNotNULL<Protocol> ProtocolPtrNN;

} // namespace SnortSam
} // namespace Trigger

#endif
