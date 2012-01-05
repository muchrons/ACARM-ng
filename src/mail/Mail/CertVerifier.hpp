/*
 * CertVerifier.hpp
 *
 */
#ifndef INCLUDE_MAIL_CERTVERIFIER_HPP_FILE
#define INCLUDE_MAIL_CERTVERIFIER_HPP_FILE

#include "Logger/Node.hpp"
#include "Mail/Vmime.hpp"
#include "Mail/Config.hpp"
#include "Mail/ExceptionInvalidCertificate.hpp"

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

#endif
