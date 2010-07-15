/*
 * File.hpp
 *
 */
#ifndef INCLUDE_LOGGER_APPENDERS_FILE_HPP_FILE
#define INCLUDE_LOGGER_APPENDERS_FILE_HPP_FILE

#include <string>
#include <fstream>

#include "Logger/Appenders/Stream.hpp"
#include "Logger/ExceptionFileAccessError.hpp"


namespace Logger
{
namespace Appenders
{

/** \brief file logger - messages are saved to disk file
 */
class File: public Stream<File>
{
public:
  /** \brief creates appender that saves to a given file.
   *  \param path path to destination file.
   */
  explicit File(const std::string &path);

  /** \brief static method for getting type name.
   *  \return compile-time pointer to type name.
   */
  static const char *getThisTypeName(void)
  {
    return "file";
  }

private:
  virtual void reinitImpl(void);

  const std::string path_;
  std::ofstream     out_;
}; // class Console

} // namespace Appenders
} // namespace Logger

#endif
