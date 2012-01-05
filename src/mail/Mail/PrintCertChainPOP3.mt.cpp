/*
 * PrintCertChain.cpp
 *
 */
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <cassert>

#include "Mail/Vmime.hpp"
#include "Mail/TestAccount.t.hpp"

using vmime::security::cert::X509Certificate;

class TestCertificateVerifier: public vmime::security::cert::certificateVerifier
{
public:
  virtual void verify(vmime::ref<vmime::security::cert::certificateChain> chain)
  {
    std::cout<<"\n"<<chain->getCount()<<" certificates in chain\n";
    vmime::utility::outputStreamAdapter os(std::cout);
    for(int i=chain->getCount()-1; i>=0; --i)
    {
      vmime::ref<X509Certificate> tmp=chain->getAt(i).dynamicCast<X509Certificate>();
      std::cout<<"------------ CERT # "<<chain->getCount()-i<<" ----------------\n";
      tmp->write(os, X509Certificate::FORMAT_PEM);
    }

  }
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
    // connect to server
    //

    std::cout<<std::endl;
    std::cout<<"connecting..."<<std::endl;

    vmime::utility::url             url("pop3s://" MAIL1_TEST_ACCOUNT_POP_SERVER);  // quick hack...
    vmime::ref<vmime::net::session> session=vmime::create<vmime::net::session>();
    session->getProperties()["transport.smtp.connection.tls"         ]="false";
    session->getProperties()["transport.smtp.connection.tls.required"]="false";
    session->getProperties()["transport.smtp.options.need-authentication"]="true";
    session->getProperties()["transport.smtp.auth.username" ]=MAIL1_TEST_ACCOUNT_LOGIN;
    session->getProperties()["transport.smtp.auth.password" ]=MAIL1_TEST_ACCOUNT_PASS;
    session->getProperties()["transport.smtp.server.address"]=MAIL1_TEST_ACCOUNT_POP_SERVER;
    session->getProperties()["transport.smtp.server.port"   ]=boost::lexical_cast<std::string>(MAIL1_TEST_ACCOUNT_POP_PORT);
    vmime::ref<vmime::net::store>       store=session->getStore(url);
    vmime::ref<TestCertificateVerifier> cv   =vmime::create<TestCertificateVerifier>();
    store->setCertificateVerifier(cv);
    store->connect();
    store->disconnect();

    std::cout<<"all done"<<std::endl;
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
