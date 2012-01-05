/*
 * MailSender.cpp
 *
 */
#include <string>
#include <cstring>
#include <boost/lexical_cast.hpp>
#include <cassert>

#include "System/ignore.hpp"
#include "Mail/MailSender.hpp"
#include "Mail/CertVerifier.hpp"
#include "Mail/MimeCreateHelper.hpp"

using namespace std;
using namespace System;

namespace Mail
{

MailSender::MailSender(const Config &cfg):
  log_("trigger.mail.mailsender"),
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
void configureOptions(const Logger::Node &log, const Config &cfg, vmime::ref<vmime::net::session> session)
{
  System::ignore(log);
  assert( session.get()!=NULL );
  // setup authorization mode
  {
    const Config::Authorization *auth=cfg.getAuthorizationConfig();
    session->getProperties()["transport.smtp.options.need-authentication"]=(auth!=NULL)?"true":"false";
    if(auth)
    {
      LOGMSG_DEBUG(log, "authentication is required");
      session->getProperties()["transport.smtp.auth.username"]=auth->user_;
      session->getProperties()["transport.smtp.auth.password"]=auth->pass_;
    }
    else
      LOGMSG_DEBUG(log, "authentication is NOT required");
  }

  const Config::Server &srv=cfg.getServerConfig();

  // setup server connection paramters
  {
    LOGMSG_DEBUG_S(log)<<"connecting to "<<srv.server_<<":"<<srv.port_;
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
    LOGMSG_DEBUG_S(log)<<"use TLS: "<<tlsMode;
    // setup in options
    session->getProperties()["transport.smtp.connection.tls"         ]=tlsMode;
    session->getProperties()["transport.smtp.connection.tls.required"]=tlsMode;
  }
} // configureOptions()


// return all recipients as one string
string toString(const Config::Recipients &r)
{
  stringstream ss;
  for(Config::Recipients::const_iterator it=r.begin(); it!=r.end(); ++it)
  {
    if( it!=r.begin() )
      ss<<" ";
    ss<<*it;
  }
  return ss.str();
} // toString()

string toString(const vmime::exception &ex)
{
  stringstream ss;
  ss<<"("<<ex.name()<<") "<<ex.what();
  return ss.str();
}

} // unnamed namespace


void MailSender::send(const std::string &subject, const std::string &content)
{
  vmime::ref<vmime::net::session>   session;
  vmime::ref<vmime::net::transport> transport;

  // connection part
  LOGMSG_DEBUG(log_, "connecting to server...");
  try
  {
    // create session
    session=vmime::create<vmime::net::session>();
    assert( session.get()!=NULL );
    configureOptions(log_, cfg_, session);
    // create transport
    const vmime::utility::url url( createURL( cfg_.getServerConfig() ) );
    transport=session->getTransport(url);
    assert( transport.get()!=NULL );
    // set certificate verifier
    typedef vmime::ref<vmime::security::cert::certificateVerifier> VerifierRef;
    VerifierRef verifier=VerifierRef::fromPtr( new CertVerifier( cfg_.getServerConfig() ) );
    transport->setCertificateVerifier(verifier);
    // connect to server (certificate will be validated along)
    LOGMSG_DEBUG_S(log_)<<"connecting to " << static_cast<std::string>(url);
    transport->connect();
    LOGMSG_DEBUG(log_, "connected!");
  }
  catch(const vmime::exception &ex)
  {
    // translate vmime-specific exception to project-wide exception
    throw ExceptionConnectionError( SYSTEM_SAVE_LOCATION,
                                    cfg_.getServerConfig().server_.c_str(),
                                    toString(ex).c_str() );
  }

  // sending message part
  LOGMSG_DEBUG(log_, "sending e-mail...");
  try
  {
    LOGMSG_DEBUG_S(log_)<<"sending to: "<<toString( cfg_.getRecipientsAddresses() );
    MimeCreateHelper             mch( cfg_.getSenderAddress(), cfg_.getRecipientsAddresses(), subject, content );
    MimeCreateHelper::MessagePtr msg=mch.createMimeMessage();
    transport->send(msg);
    LOGMSG_DEBUG(log_, "e-mail sent successfully");
  }
  catch(const vmime::exception &ex)
  {
    LOGMSG_ERROR_S(log_)<<"error sending message (to: "<<toString( cfg_.getRecipientsAddresses() )
                        <<"): "<<toString(ex);
    // translate vmime-specific exception to project-wide exception
    throw ExceptionSendingError( SYSTEM_SAVE_LOCATION,
                                 cfg_.getSenderAddress().c_str(),
                                 toString( cfg_.getRecipientsAddresses() ).c_str(),
                                 toString(ex).c_str() );
  }
}

} // namespace Mail
