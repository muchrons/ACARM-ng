/*
 * ReferenceURL.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_REFERENCEURL_HPP_FILE
#define INCLUDE_PERSISTENCY_REFERENCEURL_HPP_FILE

/* public header */

#include <boost/noncopyable.hpp>

#include "Persistency/detail/LimitedString.hpp"

namespace Persistency
{

class ReferenceURL: private boost::noncopyable
{
public:
  typedef detail::LimitedString<128> Name;
  typedef detail::LimitedString<256> URL;

  virtual ~ReferenceURL(void);

  const Name &getName(void) const;

  const URL &getURL(void) const;

protected:
  ReferenceURL(const Name &name, const URL url);

private:
  Name name_;
  URL  url_;
}; // class ReferenceURL

} // namespace Persistency

#endif
