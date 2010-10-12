/*
 * AnalyzersCreator.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_FACADES_ANALYZERSMAP_HPP_FILE
#define INCLUDE_PERSISTENCY_FACADES_ANALYZERSMAP_HPP_FILE

/* public header */

#include <boost/noncopyable.hpp>

#include "Persistency/Analyzer.hpp"
#include "Persistency/IO/Connection.hpp"
#include "Persistency/Facades/detail/LocalAnalyzersCache.hpp"

namespace Persistency
{
namespace Facades
{

/** \brief faced for creating analyzers functionality.
 *
 *  object that perform analyzers' creation, based on givne data, automatically
 *  performing ID assignment.
 *
 *  this class does not have to be unique, though the less instancess, the better
 *  performance is, since it uses internal cachinch for objects reusage.
 *
 *  \note class is NOT thread-safe.
 */
class AnalyzersCreator: private boost::noncopyable
{
public:
  /** \brief creates analyzer object, based on provided data, or creates new, if it did not exist yet.
   *  \param conn    connection use for I/O operations, if needed.
   *  \param t       transaction to oeprate on.
   *  \param name    analyzer's name.
   *  \param version analyzer's version.
   *  \param os      analyzer's OS.
   *  \param ip      analyzer's IP.
   *  \return analyzer's object instance.
   */
  AnalyzerPtrNN construct(IO::ConnectionPtrNN              conn,
                          IO::Transaction                 &t,
                          const Analyzer::Name            &name,
                          const Analyzer::Version         &version,
                          const Analyzer::OperatingSystem &os,
                          const Analyzer::IP              *ip);

private:
  detail::LocalAnalyzersCache cache_;
}; // class AnalyzersCreator

} // namespace Facades
} // namespace Persistency

#endif
