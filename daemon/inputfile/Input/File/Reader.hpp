/*
 * Reader.hpp
 *
 */
#ifndef INCLUDE_INPUT_FILE_READER_HPP_FILE
#define INCLUDE_INPUT_FILE_READER_HPP_FILE

#include <string>
#include <fstream>
#include <boost/filesystem.hpp>

#include "Commons/SharedPtrNotNULL.hpp"
#include "Input/Reader.hpp"
#include "Input/File/ExceptionFifoError.hpp"

namespace Input
{
namespace File
{

/** \brief input reader class
 */
class Reader: public Input::Reader
{
public:
  /** \brief create file reader.
   */
  Reader(const std::string &name, const boost::filesystem::path &fifoPath);

  /** \brief reads data from files.
   *  \param bf      backend facade to use for processing.
   *  \param timeout milliseconds to wait before timeout occures.
   *  \return pointer to newly allocated strucutre or NULL on timeout.
   */
  virtual DataPtr read(BackendFacade &bf, unsigned int timeout);

private:
  const boost::filesystem::path           fifoPath_;
  Commons::SharedPtrNotNULL<std::fstream> fifo_;
}; // class Reader

} // namespace File
} // namespace Input

#endif
