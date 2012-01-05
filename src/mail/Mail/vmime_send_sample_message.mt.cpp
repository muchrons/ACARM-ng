/*
 * vmime_send_sample_message.mt.cpp
 *
 */
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <cassert>

#include "Mail/Vmime.hpp"
#include "Mail/TestAccount.t.hpp"


class TestCertificateVerifier: public vmime::security::cert::certificateVerifier
{
public:
  TestCertificateVerifier(void):
    defCertVer_( vmime::create<vmime::security::cert::defaultCertificateVerifier>() )
  {
  }

  virtual void verify(vmime::ref<vmime::security::cert::certificateChain> certs)
  {
    try
    {
      defCertVer_->verify(certs);
    }
    catch(const vmime::exceptions::certificate_verification_exception &ex)
    {
      assert( certs->getCount()>0 );
      vmime::ref<vmime::security::cert::certificate>  cert=certs->getAt(0);
      vmime::security::cert::X509Certificate         *x509=
            dynamic_cast<vmime::security::cert::X509Certificate*>(cert.get());
      assert(x509!=NULL);
      std::cerr<<"/******************"<<std::endl;
      std::cerr<<"certificate verification failed ("<<ex.name()<<"): "
               <<ex.what()<<std::endl;
      std::cerr<<"type: "<<cert->getType()<<std::endl;
      vmime::utility::outputStreamAdapter osa(std::cerr);
      x509->write(osa, vmime::security::cert::X509Certificate::FORMAT_PEM);
      std::cerr<<"******************/"<<std::endl;
    }
  }

private:
  vmime::ref<vmime::security::cert::defaultCertificateVerifier> defCertVer_;
}; // class TestCertificateVerifier


int main(void)
{
  try
  {
    //
    // common settings
    //

    // set proper handler
    vmime::platform::setHandler<vmime::platforms::posix::posixHandler>();

    //
    // prepare message
    //

    // prepare message builder
    vmime::messageBuilder mb;

    // fill out content
    mb.setSubject( vmime::text("[spam] vmime test message") );
    mb.setExpeditor( vmime::mailbox(MAIL1_TEST_ACCOUNT_ADDRESS) );
    mb.getRecipients().appendAddress( vmime::create<vmime::mailbox>(MAIL2_TEST_ACCOUNT_ADDRESS) );
    mb.getTextPart()->setText( vmime::create<vmime::stringContentHandler>("test message body") );

    // get final message
    vmime::ref<vmime::message> msg=mb.construct();

    // print raw message on output
    std::cout<<"##### sending sample message. message source: #####"<<std::endl;
    vmime::utility::outputStreamAdapter out(std::cout);
    msg->generate(out);
    std::cout<<std::endl;

    //
    // sending itself
    //

    std::cout<<std::endl;
    std::cout<<"connecting..."<<std::endl;

    vmime::utility::url             url("smtp://" MAIL1_TEST_ACCOUNT_SERVER);  // quick hack...
    vmime::ref<vmime::net::session> session  =vmime::create<vmime::net::session>();
    session->getProperties()["transport.smtp.connection.tls"         ]="true";
    session->getProperties()["transport.smtp.connection.tls.required"]="true";
    session->getProperties()["transport.smtp.options.need-authentication"]="true";
    session->getProperties()["transport.smtp.auth.username" ]=MAIL1_TEST_ACCOUNT_LOGIN;
    session->getProperties()["transport.smtp.auth.password" ]=MAIL1_TEST_ACCOUNT_PASS;
    session->getProperties()["transport.smtp.server.address"]=MAIL1_TEST_ACCOUNT_SERVER;
    session->getProperties()["transport.smtp.server.port"   ]=boost::lexical_cast<std::string>(MAIL1_TEST_ACCOUNT_PORT);
    vmime::ref<vmime::net::transport>   transport=session->getTransport(url);
    vmime::ref<TestCertificateVerifier> cv       =vmime::create<TestCertificateVerifier>();
    transport->setCertificateVerifier(cv);
    transport->connect();
    std::cout<<"sending message..."<<std::endl;
    transport->send(msg);
    transport->disconnect();

    std::cout<<"sending done!"<<std::endl;
  }
  catch(const vmime::exceptions::connection_greeting_error &ex)
  {
    std::cerr<<ex.name()<<": "<<ex.what()<<std::endl;
    return 3;
  }
  catch(const vmime::exception &ex)
  {
    std::cerr<<"vmime::exception ("<<ex.name()<<"): "<<ex.what()<<std::endl;
    return 1;
  }
  catch(const std::exception &ex)
  {
    std::cerr<<"std::exception: "<<ex.what()<<std::endl;
    return 2;
  }

  return 0;
}
