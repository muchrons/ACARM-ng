/*
 * Analyzer.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_ANALYZER_HPP_FILE
#define INCLUDE_PERSISTENCY_ANALYZER_HPP_FILE

/* public header */

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/operators.hpp>

#include "Commons/SharedPtrNotNULL.hpp"
#include "Persistency/Host.hpp"
#include "Persistency/detail/LimitedString.hpp"

namespace Persistency
{
/** \brief class representing analyzer's info.
 */
class Analyzer: private boost::noncopyable,
                public  boost::equality_comparable<Analyzer>
{
public:
  /** \brief name of an analyzer.
   */
  typedef detail::LimitedString<64> Name;

  /** \brief creates analyzer.
   *  \param name name of an analyzer.
   *  \param host host analyzer runs on or NULL if not known.
   */
  Analyzer(const Name &name,
           HostPtr     host);

  /** \brief get name of an analizer.
   *  \return analizer's name.
   */
  const Name &getName(void) const;
  /** \brief gets host that analizer runs on.
   *  \return pointer to host info, or NULL if not set.
   */
  HostPtr getHost(void) const;
  /** \brief check if classes are equal.
   *  \param other element to compare with.
   *  \return true if elements are equal, false otherwise.
   */
  bool operator==(const Analyzer &other) const;

private:
  Name    name_;
  HostPtr host_;
}; // class Analyzer


/** \brief smarth pointer to analyzer class. */
typedef boost::shared_ptr<Analyzer>         AnalyzerPtr;

/** \brief smarth pointer to analyzer class, check not to be NULL. */
typedef Commons::SharedPtrNotNULL<Analyzer> AnalyzerPtrNN;

} // namespace Persistency

#endif
