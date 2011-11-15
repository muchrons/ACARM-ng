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
  /** \brief exception thrown when loop in include files has been detected.
   */
  struct ExceptionInclusionLoop: public Exception
  {
    /** \brief construct error message.
     *  \param where      location of error detection.
     *  \param reIncluded file that closes the cycle.
     */
    explicit ExceptionInclusionLoop(const Location &where, const Base::Filesystem::Path &reIncluded);
  }; // struct ExceptionInclusionLoop

  /** \brief exception thrown when invalid file is being included.
   */
  struct ExceptionInvalidInclude: public Exception
  {
    /** \brief construct error message.
     *  \param where    location of error detection.
     *  \param included include file that is invalid.
     */
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
