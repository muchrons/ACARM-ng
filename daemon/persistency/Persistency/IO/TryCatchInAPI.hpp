/*
 * TryCatchInAPI.hpp
 *
 */
#ifndef INCLUDE_PERSISNTECY_IO_TRYCATCHINAPI_HPP_FILE
#define INCLUDE_PERSISNTECY_IO_TRYCATCHINAPI_HPP_FILE

#include <cassert>

#include "Logger/Logger.hpp"
#include "Persistency/IO/Exception.hpp"

//
// NOTE: this is a helper header to be used inside cpp implementation files of
//       this module only. it constructs common try/catch for detection of
//       unhandled exception that are implementation-specific
//

namespace Persistency
{
namespace IO
{

#define TRYCATCH_BEGIN \
  try \
  {

#define TRYCATCH_END \
  } \
  catch(const Persistency::IO::Exception &ex) \
  { \
    /* IO-related exceptions are fine - proceed with handling */ \
    throw ex; \
  } \
  catch(const std::exception &ex) \
  { \
    Logger::Node log("persistency.io.alert"); \
    LOGMSG_FATAL_S(log)<<"non-IO-related exception caught (this is a bug): "<<ex.what(); \
    assert(!"non-IO-related exception cought in interface - this is a bug"); \
    throw ex;   /* fallback in case of release mode */ \
  }

} // namespace IO
} // namespace Persistency

#endif
