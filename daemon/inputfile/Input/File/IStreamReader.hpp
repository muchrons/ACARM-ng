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
  typedef std::pair<bool, std::string> Line;

  /** \brief associate with input steram.
   */
  explicit IStreamReader(std::istream &input);

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
