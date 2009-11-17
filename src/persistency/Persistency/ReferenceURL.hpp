/*
 * ReferenceURL.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_REFERENCEURL_HPP_FILE
#define INCLUDE_PERSISTENCY_REFERENCEURL_HPP_FILE

/* public header */

#include <memory>
#include <boost/noncopyable.hpp>

#include "Persistency/detail/LimitedString.hpp"

namespace Persistency
{
/** \brief represents reference URL entry.
 */
class ReferenceURL: private boost::noncopyable
{
public:
  /** \brief type of name of reference URL.
   */
  typedef detail::LimitedString<128> Name;
  /** \brief URL itself.
   */
  typedef detail::LimitedString<256> URL;
  // TODO: consider making URL a separate clas with syntax validation

  /** \brief ensure proper destruction when inherited.
   */
  virtual ~ReferenceURL(void);
  /** \brief gets reference url's name.
   *  \return name of reference URL.
   */
  const Name &getName(void) const;
  /** \brief gets URL itself.
   *  \return URL address.
   */
  const URL &getURL(void) const;

protected:
  /** \brief creates an object.
   *  \param name name of url (~description).
   *  \param url  url address itself.
   */
  ReferenceURL(const Name &name, const URL url);

private:
  Name name_;
  URL  url_;
}; // class ReferenceURL


/** \brief auto pointer to reference url type.
 */
typedef std::auto_ptr<ReferenceURL> ReferenceURLAutoPtr;

} // namespace Persistency

#endif
