/*
 * CertVerifier.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_MAIL_CERTVERIFIER_HPP_FILE
#define INCLUDE_TRIGGER_MAIL_CERTVERIFIER_HPP_FILE

#include <vmime/vmime.hpp>

#include "Logger/Node.hpp"
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
   *  \param server server address to connect to.
   */
  explicit CertVerifier(const std::string &server);
  /** \brief verify given certificate chain.
   *  \param certs certificate chain to verify.
   */
  virtual void verify(vmime::ref<vmime::security::cert::certificateChain> certs);

private:
  Logger::Node                                           log_;
  std::string                                            server_;
  vmime::ref<vmime::security::cert::certificateVerifier> defCertVer_;
}; // class CertVerifier

} // namespace Mail
} // namespace Trigger

#endif
