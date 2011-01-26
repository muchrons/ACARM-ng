/*
 * CertVerifier.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_MAIL_CERTVERIFIER_HPP_FILE
#define INCLUDE_TRIGGER_MAIL_CERTVERIFIER_HPP_FILE

#include "Logger/Node.hpp"
#include "Trigger/Mail/Vmime.hpp"
#include "Trigger/Mail/Config.hpp"
#include "Trigger/Mail/ExceptionInvalidCertificate.hpp"

namespace Trigger
{
namespace Mail
{

/** \brief implementation of certificate verifier
 */
class CertVerifier: public vmime::security::cert::certificateVerifier
{
public:
  /** \brief construct certificate verifier.
   *  \param server server's configuration.
   */
  explicit CertVerifier(const Config::Server &server);
  /** \brief verify given certificate chain.
   *  \param certs certificate chain to verify.
   */
  virtual void verify(vmime::ref<vmime::security::cert::certificateChain> certs);

private:
  typedef vmime::security::cert::defaultCertificateVerifier Verifier;
  Logger::Node         log_;
  std::string          server_;
  vmime::ref<Verifier> defCertVer_;
}; // class CertVerifier

} // namespace Mail
} // namespace Trigger

#endif
