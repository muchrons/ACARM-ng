/*
 * CertVerifier.cpp
 *
 */
#include "Logger/Logger.hpp"
#include "Commons/Filesystem/openFile.hpp"
#include "Mail/Vmime.hpp"
#include "Mail/CertVerifier.hpp"

using namespace Commons;
using namespace Commons::Filesystem;

using vmime::security::cert::X509Certificate;

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
  defCertVer_->setX509RootCAs(roots);
  defCertVer_->setX509TrustedCerts(roots);
  LOGMSG_DEBUG(log_, "all done");
}

void CertVerifier::verify(vmime::ref<vmime::security::cert::certificateChain> chain)
{
  LOGMSG_DEBUG(log_, "verifying certificate chain");
  try
  {
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
