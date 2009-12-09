/*
 * ReferenceURL.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_POSTGRES_REFERENCEURL_HPP_FILE
#define INCLUDE_PERSISTENCY_POSTGRES_REFERENCEURL_HPP_FILE

#include "Persistency/ReferenceURL.hpp"

namespace Persistency
{
namespace Postgres
{
/** \brief represents reference URL entry.
 */
class ReferenceURL: public Persistency::ReferenceURL
{
public:
  /** \brief creates an object.
   *  \param name name of url (~description).
   *  \param url  url address itself.
   */
  ReferenceURL(const Name &name, const URL &url);
}; // class ReferenceURL

} // namespace Postgres
} // namespace Persistency

#endif
