/*
 * MetaAlert.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_METAALERT_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_METAALERT_HPP_FILE

/* public header */

#include <boost/noncopyable.hpp>

#include "Persistency/Alert.hpp"

namespace Persistency
{
namespace IO
{

/** \brief operations on Persistency::MetaAlert API
 */
class MetaAlert: private boost::noncopyable
{
public:
  virtual ~MetaAlert(void);
  // TODO
}; // class MetaAlert

} // namespace IO
} // namespace Persistency

#endif
