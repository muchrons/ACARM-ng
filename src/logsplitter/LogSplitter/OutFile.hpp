/*
 * OutFile.hpp
 *
 */
#ifndef INCLUDE_LOGSPLITTER_OUTFILE_HPP_FILE
#define INCLUDE_LOGSPLITTER_OUTFILE_HPP_FILE

#include <string>
#include <fstream>
#include <stdexcept>

#include <boost/noncopyable.hpp>


namespace LogSplitter
{
// foraward declaration
class OutFile;
}

namespace std
{
/** \brief endl marker.
 *  \param strm stream to operate on.
 *  \return stream reference.
 */
LogSplitter::OutFile &endl(LogSplitter::OutFile &strm);
} // namespace std



namespace LogSplitter
{

/** \brief class performing writing to an output buffer.
 */
class OutFile: private boost::noncopyable
{
public:
  /** \brief save to a given file.
   *  \param file file name to write to.
   */
  explicit OutFile(const std::string &file);

  /** \brief append data to stream.
   *  \param t data to be added to stream.
   *  \return self-reference.
   */
  template<typename T>
  OutFile &operator<<(const T &t)
  {
    file_<<t;
    return *this;
  }

  /** \brief manipulator appliance (for endl marker).
   *  \param manip manipulator to use.
   *  \return stream reference.
   */
  OutFile &operator<<(OutFile& (*manip)(OutFile&))
  {
    if(manip==NULL)
      throw std::runtime_error("NULL manipulator given");
    return manip(*this);
  }

  /** \brief flush buffers.
   *  \note it is NOT performed by the d-tor, since it may cause out-of-disk
   *        or disk-writes errors that would be then ignored.
   */
  void flush(void);

private:
  friend OutFile &std::endl(OutFile &strm);

  std::ofstream file_;
}; // class Parser

} // namespace LogSplitter



namespace std
{
// implementation of endl manipulator
inline LogSplitter::OutFile &endl(LogSplitter::OutFile &strm)
{
  strm.file_<<std::endl;
  return strm;
}
} // namespace std

#endif
