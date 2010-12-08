/*
 * CertVerifier.cpp
 *
 */
#include <vmime/vmime.hpp>
#include <iostream>                 

#include "Logger/Logger.hpp"
#include "Commons/Filesystem/openFile.hpp"
#include "Trigger/Mail/CertVerifier.hpp"

using namespace Commons;
using namespace Commons::Filesystem;

using vmime::security::cert::X509Certificate;

namespace Trigger
{
namespace Mail
{

namespace
{
typedef vmime::ref<X509Certificate> CertPtr;

CertPtr certFromFile(const boost::filesystem::path &file)
{
  SharedPtrNotNULL<std::istream>     in  =openFile(file, Mode::READ);
  vmime::utility::inputStreamAdapter vin(*in);
  CertPtr                            cert=X509Certificate::import(vin);
  assert( cert.get()!=NULL );
  return cert;
} // certFromFile()
} // unnamed namespace

CertVerifier::CertVerifier(const Config::Server &server):
  log_("trigger.mail.certverifier"),
  server_(server.server_),
  defCertVer_( vmime::create<Verifier>() )
{
  LOGMSG_INFO_S(log_)<<"reading certifiacte from file "<<server.rootCAcertPath_;
  LOGMSG_DEBUG(log_, "setting certificate as root CA");
  std::vector<CertPtr> roots;
  roots.push_back( certFromFile(server.rootCAcertPath_) );
  roots.push_back( certFromFile("testdata/1.pem") );
  roots.push_back( certFromFile("testdata/2.pem") );
  roots.push_back( certFromFile("testdata/3.pem") );
  roots.push_back( certFromFile("testdata/A.pem") );
  roots.push_back( certFromFile("testdata/B.pem") );
  defCertVer_->setX509RootCAs(roots);
  defCertVer_->setX509TrustedCerts(roots);
  LOGMSG_DEBUG(log_, "all done");
}

void CertVerifier::verify(vmime::ref<vmime::security::cert::certificateChain> chain)
{
  LOGMSG_DEBUG(log_, "verifying certificate chain");
  try
  {
    std::cout<<"\n"<<chain->getCount()<<" certificates in chain\n";
    for(int i=0; i<chain->getCount(); ++i)
    {
      vmime::utility::outputStreamAdapter os(std::cout);
      CertPtr tmp=chain->getAt(i).dynamicCast <X509Certificate>();
      /*
      const vmime::byteArray b=tmp->getEncoded();
      for(vmime::byteArray::const_iterator it=b.begin(); it!=b.end(); ++it)
      {
        const char transl[]="0123456789ABCDEF";
        std::cout<< transl[ (*it&0xF0)>>4 ];
        std::cout<< transl[ (*it&0x0F)>>0 ];
        std::cout<< (isprint(*it)?*it:'?');
      }
      */
      std::cout<<"------------ CERT # "<<i<<" ----------------\n";
      std::cout<<"TYPE: "<<tmp->getType()<<std::endl;
      tmp->write(os, X509Certificate::FORMAT_PEM);
    }

    defCertVer_->verify(chain);
  }
  catch(const vmime::exceptions::certificate_verification_exception &ex)
  {
    LOGMSG_ERROR_S(log_)<<"certificate verification failed: "<<ex.what();
    // re-throw this in order to inform on error type
    throw ExceptionInvalidCertificate( SYSTEM_SAVE_LOCATION, server_.c_str(), ex.what() );
  }
  LOGMSG_INFO(log_, "certificate chain accepted");
}

} // namespace Mail
} // namespace Trigger
