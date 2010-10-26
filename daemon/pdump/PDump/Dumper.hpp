/*
 * Dumper.hpp
 *
 */
#ifndef INCLUDE_PDUMP_DUMPER_HPP_FILE
#define INCLUDE_PDUMP_DUMPER_HPP_FILE

#include <iosfwd>
#include <boost/filesystem.hpp>
#include <boost/noncopyable.hpp>

#include "Persistency/IO/Restorer.hpp"

namespace PDump
{

/** \brief class that reads nodes and saves them to files.
 *  \note for now only alerts are saved.
 */
class Dumper: private boost::noncopyable
{
public:
  struct Stats
  {
    Stats(size_t total, size_t attempts, size_t writes);
    size_t getTotal(void) const;
    size_t getWrites(void) const;
    size_t getAttempts(void) const;

  private:
    size_t total_;
    size_t writes_;
    size_t attempts_;
  }; // struct Stats

  /** \brief typedef on nodes vector. */
  typedef Persistency::IO::Restorer::NodesVector NodesVector;

  /** \brief initialize object, with a proper output streams.
   *  \param out    stream for outputing normal events.
   *  \param errOut stream for outputing error events.
   */
  Dumper(std::ostream &out, std::ostream &errOut);

  /** \brief restores all meta-alerts in given time range.
   *  \param from     time to start dumping from.
   *  \param to       time to stop dumping at.
   *  \param nodesOut output collection.
   */
  void restoreBetween(const Persistency::Timestamp  from,
                      const Persistency::Timestamp  to,
                      NodesVector                  &nodesOut);

  /** \brief outputs nodes to a given directory.
   *  \param nodes  nodes to be saved as files.
   *  \param outDir output directory to use.
   *  \return statistics of operation.
   */
  Stats writeToDir(const NodesVector &nodes, const boost::filesystem::path &outDir);

private:
  std::ostream &out_;
  std::ostream &errOut_;
}; // class Dumper

} // namespace PDump

#endif
