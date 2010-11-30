/*
 * MailSender.cpp
 *
 */
#include <string>
#include <cstring>
#include <boost/lexical_cast.hpp>
#include <cassert>

#include "Trigger/Mail/MailSender.hpp"
#include "Trigger/Mail/MimeCreateHelper.hpp"

using namespace std;
using namespace System;

namespace Trigger
{
namespace Mail
{

MailSender::MailSender(const Config &cfg):
  cfg_(cfg)
{
}

namespace
{

string createProtocolPrefix(const Config::Server::Protocol p, const Config::Server::Security s)
{
  string out;
  // select protocol
  switch( p.toInt() )
  {
    case Config::Server::Protocol::SMTP:
      out+="smtp";
      break;
    default:
      assert(!"unknown protocol - code is not updated");
      break;
  } // switch(protocol)

  // select security
  switch( s.toInt() )
  {
    case Config::Server::Security::TLS:
      // TLS is configured in options
      break;
    case Config::Server::Security::SSL:
      out+="s";
      break;
    default:
      assert(!"unknown security option - code is not updated");
      break;
  } // switch(security)

  // return response
  return out;
} // createProtocolPrefix()


// create URL for vmime
std::string createURL(const Config::Server &srv)
{
  const string proto=createProtocolPrefix(srv.proto_, srv.sec_);
  return proto + "://" + srv.server_;
} // createURL()

// configure additional options
void configureOptions(const Config &cfg, vmime::ref<vmime::net::session> session)
{
  assert( session.get()!=NULL );
  // setup authorization mode
  {
    const Config::Authorization *auth=cfg.getAuthorizationConfig();
    session->getProperties()["transport.smtp.options.need-authentication"]=(auth!=NULL)?"true":"false";
    if(auth)
    {
      session->getProperties()["transport.smtp.auth.username"]=auth->user_;
      session->getProperties()["transport.smtp.auth.password"]=auth->pass_;
    }
  }

  const Config::Server &srv=cfg.getServerConfig();

  // setup server connection paramters
  {
    session->getProperties()["transport.smtp.server.address"]=srv.server_;
    session->getProperties()["transport.smtp.server.port"   ]=boost::lexical_cast<string>(srv.port_);
  }

  // check if encryption is set to TLS
  {
    const char *tlsMode=NULL;
    if( srv.sec_==Config::Server::Security::TLS )
      tlsMode="true";
    else
      tlsMode="false";
    // setup in options
    session->getProperties()["transport.smtp.connection.tls"         ]=tlsMode;
    session->getProperties()["transport.smtp.connection.tls.required"]=tlsMode;
  }
} // configureOptions()

} // unnamed namespace


void MailSender::send(const std::string &subject, const std::string &content)
{
  vmime::ref<vmime::net::session>   session;
  vmime::ref<vmime::net::transport> transport;

  // connection part
  try
  {
    // create session
    session=vmime::create<vmime::net::session>();
    assert( session.get()!=NULL );
    // create transport
    const vmime::utility::url url( createURL( cfg_.getServerConfig() ) );
    transport=session->getTransport(url);
    assert( transport.get()!=NULL );
    // connect to server (certificate will be validatd along)
    if( !transport->isConnected() )
      transport->connect();
  }
  catch(const vmime::exception &ex)
  {
    // translate vmime-specific exception to project-wide exception
    throw ExceptionConnectionError(SYSTEM_SAVE_LOCATION, cfg_.getServerConfig().server_.c_str(), ex.what() );
  }

  // sending message part
  try
  {
    // TODO
  }
  catch(const vmime::exception &ex)
  {
    // TODO
    // translate vmime-specific exception to project-wide exception
    //throw ExceptionSendingError(SYSTEM_SAVE_LOCATION, ex.what() );
  }

  // TODO....
#if 0

  // sending itself
    transport_->send(msg);  // send message


    /** \brief certificate verifier that accepts all certifiactes.
     *  \note this verifier tries to verify certificate, but in case of failure
     *        it accepts it anyhow, generating proper log entry onyl.
     */
    class AcceptAll: public vmime::security::cert::certificateVerifier
  { 
    public:
      /** \brief construct certificate verifier.
       */
      AcceptAll(void);
      /** \brief verify given certificate chain.
       *  \param certs certificate chain to verify.
       */
      virtual void verify(vmime::ref<vmime::security::cert::certificateChain> certs);

    private:
      Logger::LoggerFac                                             log_;
      vmime::ref<vmime::security::cert::defaultCertificateVerifier> defCertVer_;
  }; // class AcceptAll

    AcceptAll::AcceptAll(void):
      log_("MailSender.Certs.AcceptAll"),
      defCertVer_( vmime::create<vmime::security::cert::defaultCertificateVerifier>() )
  {
  }

    void AcceptAll::verify(vmime::ref<vmime::security::cert::certificateChain> certs)
    {
      log_.info("verifying certificate with default verifier");
      try
      {
        defCertVer_->verify(certs);
      }
      catch(const vmime::exceptions::certificate_verification_exception &ex)
      {
        log_.error("certificate verification failed - connection is NOT secure");
        // due to nature of this verifier, we ignore error
      }
      log_.info("certificate accepted");
    }

#endif
}

} // namespace Mail
} // namespace Trigger
