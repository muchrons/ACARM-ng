/*
 * Analyzer.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_ANALYZER_HPP_FILE
#define INCLUDE_PERSISTENCY_ANALYZER_HPP_FILE

/* public header */

#include <boost/noncopyable.hpp>
#include <boost/operators.hpp>

#include "Commons/SharedPtrNotNULL.hpp"
#include "Persistency/IPTypes.hpp"
#include "Persistency/detail/NullValue.hpp"
#include "Persistency/detail/LimitedString.hpp"

namespace Persistency
{
/** \brief class representing analyzer's info.
 */
class Analyzer: private boost::noncopyable,
                public  boost::equality_comparable<Analyzer>,
                public  IPTypes<Analyzer>
{
public:
  /** \brief name of an analyzer. */
  typedef detail::LimitedString<128> Name;
  /** \brief analyzer's version. */
  typedef detail::LimitedString<16>  Version;
  /** \brief os analyzer runs on. */
  typedef detail::LimitedString<128> OS;

  /** \brief creates analyzer.
   *  \param name    name of an analyzer.
   *  \param version analyzer's version.
   *  \param os      operating system name/version.
   *  \param ip      IP analyzer's running on.
   */
  Analyzer(const Name    &name,
           const Version *version,
           const OS      *os,
           const IP      *ip);

  /** \brief get name of an analizer.
   *  \return analizer's name.
   */
  const Name &getName(void) const;
  /** \brief gets analyzer's version.
   *  \return analyzer version or NULL.
   */
  const Version *getVersion(void) const;
  /** \brief gets OS's name/version.
   *  \return OS name/version or NULL.
   */
  const OS *getOS(void) const;
  /** \brief gets analyzer's IP.
   *  \return IP of analyzer or NULL.
   */
  const IP *getIP(void) const;
  /** \brief check if classes are equal.
   *  \param other element to compare with.
   *  \return true if elements are equal, false otherwise.
   */
  bool operator==(const Analyzer &other) const;

private:
  Name                       name_;
  detail::NullValue<Version> version_;
  detail::NullValue<OS>      os_;
  detail::NullValue<IP>      ip_;
}; // class Analyzer

/** \brief smarth pointer to analyzer class, check not to be NULL. */
typedef Commons::SharedPtrNotNULL<Analyzer> AnalyzerPtrNN;

} // namespace Persistency

#endif
