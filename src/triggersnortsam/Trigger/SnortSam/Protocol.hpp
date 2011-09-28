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
  Protocol(void);
  virtual ~Protocol(void);

  void block(const Config::IP &from, const Config::IP &to);
  void deinit(void);

protected:
  const std::string &getProtocolVersion(void);
  bool isConnected(void);
  void init(void);

  const Logger::Node log_;

private:
  virtual const std::string &getProtocolVersionImpl(void) = 0;
  virtual bool isConnectedImpl(void) = 0;
  virtual void initImpl(void) = 0;
  virtual void deinitImpl(void) = 0;
  virtual void blockImpl(const Config::IP &from, const Config::IP &to) = 0;
}; // class Protocol


/** \brief pointer to the protocol's implementation. */
typedef Commons::SharedPtrNotNULL<Protocol> ProtocolPtrNN;

} // namespace SnortSam
} // namespace Trigger

#endif
