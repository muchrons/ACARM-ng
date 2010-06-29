/*
 * Analyzer.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_ANALYZER_HPP_FILE
#define INCLUDE_PERSISTENCY_ANALYZER_HPP_FILE

/* public header */

#include <boost/noncopyable.hpp>
#include <boost/operators.hpp>
#include <inttypes.h>

#include "Base/NullValue.hpp"
#include "Commons/SharedPtrNotNULL.hpp"
#include "Commons/LimitedString.hpp"
#include "Commons/LimitedNULLString.hpp"
#include "Persistency/IPTypes.hpp"

namespace Persistency
{
/** \brief class representing analyzer's info.
 */
class Analyzer: private boost::noncopyable,
                public  boost::equality_comparable<Analyzer>,
                public  IPTypes<Analyzer>
{
public:
  /** \brief object-id type. */
  typedef uint64_t                        ID;
  /** \brief name of an analyzer. */
  typedef Commons::LimitedString<128>     Name;
  /** \brief analyzer's version. */
  typedef Commons::LimitedNULLString<16>  Version;
  /** \brief os analyzer runs on. */
  typedef Commons::LimitedNULLString<128> OperatingSystem;

  /** \brief creates analyzer.
   *  \param name    name of an analyzer.
   *  \param version analyzer's version.
   *  \param os      operating system name/version.
   *  \param ip      IP analyzer's running on.
   */
  Analyzer(const Name            &name,
           const Version         &version,
           const OperatingSystem &os,
           const IP              *ip);

  /** \brief get name of an analizer.
   *  \return analizer's name.
   */
  const Name &getName(void) const;
  /** \brief gets analyzer's version.
   *  \return analyzer version or NULL.
   */
  const Version &getVersion(void) const;
  /** \brief gets OS's name/version.
   *  \return OS name/version or NULL.
   */
  const OperatingSystem &getOperatingSystem(void) const;
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
  Name                name_;
  Version             version_;
  OperatingSystem     os_;
  Base::NullValue<IP> ip_;
}; // class Analyzer

/** \brief smarth pointer to analyzer class, check not to be NULL. */
typedef Commons::SharedPtrNotNULL<Analyzer> AnalyzerPtrNN;

} // namespace Persistency

#endif
