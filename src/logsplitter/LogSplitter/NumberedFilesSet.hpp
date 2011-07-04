/*
 * NumberedFilesSet.hpp
 *
 */
#ifndef INCLUDE_LOGSPLITTER_NUMBEREDFILESSET_HPP_FILE
#define INCLUDE_LOGSPLITTER_NUMBEREDFILESSET_HPP_FILE

#include <string>
#include <vector>
#include <fstream>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

namespace LogSplitter
{

/** \brief collection returning new file with a given number assigned.
 */
class NumberedFilesSet: private boost::noncopyable
{
public:
  typedef boost::shared_ptr<std::ofstream> FileStreamPtr;

  explicit NumberedFilesSet(const std::string &prefix);

  FileStreamPtr get(unsigned int n);

private:
  typedef std::vector<FileStreamPtr> Outputs;

  std::string makeFileName(unsigned int n);

  const std::string prefix_;
  Outputs           out_;
}; // class NumberedFilesSet

} // namespace LogSplitter

#endif
