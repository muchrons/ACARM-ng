/*
 * Dumper.hpp
 *
 */
#ifndef INCLUDE_PDUMP_DUMPER_HPP_FILE
#define INCLUDE_PDUMP_DUMPER_HPP_FILE

#include <iosfwd>
#include <utility>
#include <boost/filesystem.hpp>
#include <boost/noncopyable.hpp>

#include "Persistency/IO/Restorer.hpp"

namespace PDump
{

class Dumper: private boost::noncopyable
{
public:
  typedef Persistency::IO::Restorer::NodesVector NodesVector;

  Dumper(std::ostream &out, std::ostream &errOut);

  void restoreBetween(const Persistency::Timestamp  from,
                      const Persistency::Timestamp  to,
                      NodesVector                  &nodesOut);

  std::pair<int, int> writeToDir(const NodesVector &nodes, const boost::filesystem::path &outDir);

private:
  std::ostream &out_;
  std::ostream &errOut_;
}; // class Dumper

} // namespace PDump

#endif
