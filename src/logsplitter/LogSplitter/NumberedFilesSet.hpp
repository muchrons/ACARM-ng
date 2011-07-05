/*
 * NumberedFilesSet.hpp
 *
 */
#ifndef INCLUDE_LOGSPLITTER_NUMBEREDFILESSET_HPP_FILE
#define INCLUDE_LOGSPLITTER_NUMBEREDFILESSET_HPP_FILE

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

#include "LogSplitter/OutFile.hpp"

namespace LogSplitter
{

/** \brief collection returning new file with a given number assigned.
 */
class NumberedFilesSet: private boost::noncopyable
{
public:
  /** \brief output stream pointer. */
  typedef boost::shared_ptr<OutFile> FileStreamPtr;

  /** \brief dynamic set of numeberd files, named with a given prefix.
   *  \param prefix prefix for each file name.
   */
  explicit NumberedFilesSet(const std::string &prefix);

  /** \brief get stream for a given file number. is it is not present, it is created.
   *  \param n number for a given file.
   *  \return non-NULL pointer to the output stream.
   */
  FileStreamPtr get(unsigned int n);

private:
  typedef std::vector<FileStreamPtr> Outputs;

  std::string makeFileName(unsigned int n);

  const std::string prefix_;
  Outputs           out_;
}; // class NumberedFilesSet

} // namespace LogSplitter

#endif
