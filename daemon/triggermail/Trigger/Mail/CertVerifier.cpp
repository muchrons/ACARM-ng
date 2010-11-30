/*
 * CertVerifier.cpp
 *
 */
#include <vmime/vmime.hpp>

#include "Logger/Logger.hpp"
#include "Trigger/Mail/CertVerifier.hpp"

namespace Trigger
{
namespace Mail
{


CertVerifier::CertVerifier(const std::string &server):
  log_("trigger.mail.certverifier"),
  server_(server),
  defCertVer_( vmime::create<vmime::security::cert::defaultCertificateVerifier>() )
{
}

void CertVerifier::verify(vmime::ref<vmime::security::cert::certificateChain> certs)
{
  LOGMSG_DEBUG(log_, "verifying certificate chain");
  try
  {
    defCertVer_->verify(certs);
  }
  catch(const vmime::exceptions::certificate_verification_exception &ex)
  {
    LOGMSG_ERROR_S(log_)<<"certificate verification failed: "<<ex.what();
    // re-throw this in order to inform on error type
    //throw ExceptionInvalidCertificate( SYSTEM_SAVE_LOCATION, server_.c_str(), ex.what() );
#warning TODO: implement real certificate validation
  }
  LOGMSG_INFO(log_, "certificate chain accepted");
}

} // namespace Mail
} // namespace Trigger
