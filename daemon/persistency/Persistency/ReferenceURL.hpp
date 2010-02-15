/*
 * ReferenceURL.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_REFERENCEURL_HPP_FILE
#define INCLUDE_PERSISTENCY_REFERENCEURL_HPP_FILE

/* public header */

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/operators.hpp>

#include "Commons/SharedPtrNotNULL.hpp"
#include "Persistency/detail/LimitedString.hpp"

namespace Persistency
{
/** \brief represents reference URL entry.
 */
class ReferenceURL: private boost::noncopyable,
                    public  boost::operators<ReferenceURL>
{
public:
  /** \brief type of name of reference URL.
   */
  typedef detail::LimitedString<128> Name;
  /** \brief URL itself.
   */
  typedef detail::LimitedString<256> URL;

  /** \brief creates an object.
   *  \param name name of url (~description).
   *  \param url  url address itself.
   */
  ReferenceURL(const Name &name, const URL &url);

  /** \brief gets reference url's name.
   *  \return name of reference URL.
   */
  const Name &getName(void) const;
  /** \brief gets URL itself.
   *  \return URL address.
   */
  const URL &getURL(void) const;
  /** \brief check if classes are equal.
   *  \param other element to compare with.
   *  \return true if elements are equal, false otherwise.
   */
  bool operator==(const ReferenceURL &other) const;

private:
  Name name_;
  URL  url_;
}; // class ReferenceURL


/** \brief smart pointer to reference url type. */
typedef boost::shared_ptr<ReferenceURL>         ReferenceURLPtr;

/** \brief smart pointer to reference url type, not NULL. */
typedef Commons::SharedPtrNotNULL<ReferenceURL> ReferenceURLPtrNN;

} // namespace Persistency

#endif
