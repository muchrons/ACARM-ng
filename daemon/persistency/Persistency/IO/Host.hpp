/*
 * Host.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_HOST_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_HOST_HPP_FILE

/* public header */

#include <boost/noncopyable.hpp>

#include "Persistency/Host.hpp"

namespace Persistency
{
namespace IO
{

/** \brief operations on Persistency::Host API
 */
class Host: private boost::noncopyable
{
public:
  virtual ~Host(void);
  // TODO
}; // class Host

} // namespace IO
} // namespace Persistency

#endif
