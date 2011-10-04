/*
 * Protocol.cpp
 *
 */
#include "Commons/Convert.hpp"
#include "Trigger/SnortSam/Ver14/Protocol.hpp"


namespace Trigger
{
namespace SnortSam
{
namespace Ver14
{

Protocol::Protocol(const Who             who,
                   const How             how,
                   const unsigned int    duration,
                   std::auto_ptr<NetIO>  netIO,
                   std::auto_ptr<Crypto> crypto):
  who_(who),
  how_(how),
  duration_(duration),
  ver_(14),
  verStr_( Commons::Convert::to<std::string>(ver_) ),
  connected_(false),
  netIO_( netIO.release() ),
  crypto_( crypto.release() )
{
  if(netIO_.get()==NULL)
    throw Exception(SYSTEM_SAVE_LOCATION, "network IO must be set");
  if(crypto_.get()==NULL)
    throw Exception(SYSTEM_SAVE_LOCATION, "cryptographical API must be set");
}

const std::string &Protocol::getProtocolVersionImpl(void)
{
  return verStr_;
}

bool Protocol::isConnectedImpl(void)
{
  return connected_;
}

void Protocol::initImpl(void)
{
  // TODO
}

void Protocol::deinitImpl(void)
{
  // TODO
}

void Protocol::blockImpl(const Config::IP &/*from*/, const Config::IP &/*to*/)
{
  // TODO
}

} // namespace Ver14
} // namespace SnortSam
} // namespace Trigger
