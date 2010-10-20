/*
 * ParserCallback.hpp
 *
 */
#ifndef INCLUDE_INPUT_FILE_PARSERCALLBACK_HPP_FILE
#define INCLUDE_INPUT_FILE_PARSERCALLBACK_HPP_FILE

#include <fstream>
#include <boost/filesystem.hpp>

#include "Commons/SharedPtrNotNULL.hpp"
#include "Persistency/GraphNodePtr.hpp"
#include "Input/BackendFacade.hpp"

namespace Input
{
namespace File
{

/** \brief input reader class
 */
class ParserCallback: public BackendFacade::CustomIOInterface
{
public:
  /** \brief 
   */
  explicit ParserCallback(const boost::filesystem::path &inputPath);

  virtual void customAction(Persistency::IO::ConnectionPtrNN conn, Persistency::IO::Transaction &t);

private:
  Commons::SharedPtrNotNULL<std::fstream> input_;
  Persistency::GraphNodePtr               node_;
}; // class ParserCallback

} // namespace File
} // namespace Input

#endif
