/*
 * OutFile.hpp
 *
 */
#ifndef INCLUDE_LOGSPLITTER_OUTFILE_HPP_FILE
#define INCLUDE_LOGSPLITTER_OUTFILE_HPP_FILE

#include <iostream>             
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cassert>

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>


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
   *  \param file  file name to write to.
   *  \param bytes minimal size of buffer, before automatic flushing (in bytes).
   */
  OutFile(const std::string &file, unsigned int bytes);

  /** \brief append data to stream.
   *  \param t data to be added to stream.
   *  \return self-reference.
   */
  template<typename T>
  OutFile &operator<<(const T &t)
  {
    assert(buf_.get()!=NULL);
    (*buf_)<<t;
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

  // cals flush, when bufers are filled enough
  void rareFlush(void)
  {
    assert(buf_.get()!=NULL);
    if(buf_->tellp()>=bytes_)
      flush();
  }

  std::ofstream                        file_;
  boost::scoped_ptr<std::stringstream> buf_;
  const unsigned int                   bytes_;
}; // class Parser

} // namespace LogSplitter



namespace std
{
// implementation of endl manipulator
inline LogSplitter::OutFile &endl(LogSplitter::OutFile &strm)
{
  assert(strm.buf_.get()!=NULL);
  (*strm.buf_)<<std::endl;
  strm.rareFlush();
  return strm;
} // endl()
} // namespace std

#endif
