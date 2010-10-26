/*
 * ParserCallback.hpp
 *
 */
#ifndef INCLUDE_INPUT_FILE_PARSERCALLBACK_HPP_FILE
#define INCLUDE_INPUT_FILE_PARSERCALLBACK_HPP_FILE

#include <fstream>
#include <boost/filesystem.hpp>

#include "Commons/SharedPtrNotNULL.hpp"
#include "Commons/Filesystem/Exception.hpp"
#include "Persistency/Alert.hpp"
#include "Persistency/IO/Connection.hpp"
#include "Persistency/IO/Transaction.hpp"
#include "RFCIO/IDMEF/Exception.hpp"
#include "RFCIO/XML/Exception.hpp"
#include "Input/BackendFacade.hpp"

namespace Input
{
namespace File
{

/** \brief input (file) reader class.
 */
class ParserCallback: public BackendFacade::CustomIOInterface
{
public:
  /** \brief open given input file.
   *  \param inputPath path to file to be opened.
   */
  explicit ParserCallback(const boost::filesystem::path &inputPath);
  /** \brief parse opened file and save result.
   *  \param conn connection to be used by internal operations.
   *  \param t    transaction to be used.
   */
  virtual void customAction(Persistency::IO::ConnectionPtrNN conn, Persistency::IO::Transaction &t);
  /** \brief returns paresed alert.
   *  \return alert parsed from file.
   */
  Persistency::AlertPtrNN getAlert(void) const;

private:
  Commons::SharedPtrNotNULL<std::fstream> input_;
  Persistency::AlertPtr                   alert_;
}; // class ParserCallback

} // namespace File
} // namespace Input

#endif
