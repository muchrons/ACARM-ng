/*
 * FileReader.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_FILEREADER_HPP_FILE
#define INCLUDE_CONFIGIO_FILEREADER_HPP_FILE

#include <string>
#include <boost/filesystem.hpp>
#include <boost/scoped_array.hpp>

#include "System/DiskFile.hpp"
#include "ConfigIO/ExceptionFileAccessError.hpp"


namespace ConfigIO
{

/** \brief reads file from disk to the memory and returns as byte array/string.
 */
class FileReader
{
public:
  /** \brief byte representation.
   */
  typedef unsigned char Byte;

  /** \brief reads file given as a parameter from disk.
   *  \param path fiel to be read.
   *  \note read string is always null-terminated, so when text file is being
   *        read it can be threated as regular C-string (see getString()
   *        method). if file is binary, null terminator may appear before
   *        acctual end of data (use getSize() in case of binary data).
   */
  explicit FileReader(const boost::filesystem::path &path);

  /** \brief returns pointer to byte array of read data.
   *  \return pointer to data read.
   */
  const Byte *getData(void) const
  {
    return data_.get();
  }

  /** \brief gets access to data read as C-stype string.
   *  \return C-style string with data.
   *  \note this is usefull when read file is text file.
   */
  const char *getString(void) const
  {
    return static_cast<const char*>( static_cast<const void*>( getData() ) );
  }

  /** \brief gets number of bytes read.
   *  \return size of data in bytes.
   */
  size_t getSize(void) const
  {
    return size_;
  }

private:
  boost::scoped_array<Byte> data_;
  size_t                    size_;
}; // class FileReader

} // namespace ConfigIO

#endif
