/*
 * ConfigFileReader.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_CONFIGFILEREADER_HPP_FILE
#define INCLUDE_CONFIGIO_CONFIGFILEREADER_HPP_FILE

#include "XML/Tree.hpp"
#include "Base/Filesystem/BoostFS.hpp"
#include "ConfigIO/ExceptionFileAccessError.hpp"


namespace ConfigIO
{

/** \brief reads config file from disk and parse it as XML tree.
 */
class ConfigFileReader
{
public:
  struct ExceptionInclusionLoop: public Exception
  {
    explicit ExceptionInclusionLoop(const Location &where, const Base::Filesystem::Path &reIncluded);
  }; // struct ExceptionInclusionLoop

  struct ExceptionInvalidInclude: public Exception
  {
    explicit ExceptionInvalidInclude(const Location &where, const Base::Filesystem::Path &included);
  }; // struct ExceptionInvalidInclude

  /** \brief reads file given as a parameter from disk.
   *  \param path path to file to be read.
   *  \note this call automatically expands includes inside the config file.
   */
  explicit ConfigFileReader(const boost::filesystem::path &path);

  /** \brief returns pointer to byte array of read data.
   *  \return pointer to data read.
   */
  const XML::Tree &getTree(void) const
  {
    return root_;
  }

private:
  XML::Tree root_;
}; // class ConfigFileReader

} // namespace ConfigIO

#endif
