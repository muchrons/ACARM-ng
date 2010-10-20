/*
 * IStreamReader.hpp
 *
 */
#ifndef INCLUDE_INPUT_FILE_ISTREAMREADER_HPP_FILE
#define INCLUDE_INPUT_FILE_ISTREAMREADER_HPP_FILE

#include <iosfwd>
#include <string>
#include <utility>
#include <sstream>
#include <boost/scoped_ptr.hpp>
#include <boost/noncopyable.hpp>

#include "Input/File/Exception.hpp"

namespace Input
{
namespace File
{

/** \brief reader from stream, that implement timeout.
 */
class IStreamReader: private boost::noncopyable
{
public:
  /** \brief response from stream. bool informs if read succeded and string gives read value. */
  typedef std::pair<bool, std::string> Line;

  /** \brief associate with input steram.
   *  \param input stream to read from.
   */
  explicit IStreamReader(std::istream &input);

  /** \brief read line from stream.
   *  \param timeout maximum ammount of time to wait for data.
   *  \return pair <true, line_content> on cussecc or <false, unset> on error.
   */
  Line readLine(unsigned int timeout);

private:
  void truncateStringstreamBy(const size_t len);

  std::istream                         &input_;
  char                                  tmp_[1024];
  boost::scoped_ptr<std::stringstream>  ss_;
}; // class Reader

} // namespace File
} // namespace Input

#endif
