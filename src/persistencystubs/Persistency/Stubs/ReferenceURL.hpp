/*
 * ReferenceURL.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_STUBS_REFERENCEURL_HPP_FILE
#define INCLUDE_PERSISTENCY_STUBS_REFERENCEURL_HPP_FILE

/* public header */

#include "Persistency/ReferenceURL.hpp"

namespace Persistency
{
namespace Stubs
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
  ReferenceURL(const Name &name, const URL url):
    Persistency::ReferenceURL(name, url)
  {
  }

  /** \brief creates default object.
   */
  ReferenceURL(void):
    Persistency::ReferenceURL("GNU", "http://www.gnu.org")
  {
  }
}; // class ReferenceURL

} // namespace Stubs
} // namespace Persistency

#endif
