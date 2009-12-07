/*
 * GraphOperations.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_GRAPHOPERATIONS_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_GRAPHOPERATIONS_HPP_FILE

/* public header */

#include <boost/noncopyable.hpp>

//#include "Persistency/Alert.hpp"

namespace Persistency
{
namespace IO
{

/** \brief operations on Persistency::Graph* API
 */
class Graph: private boost::noncopyable
{
public:
  virtual ~Graph(void);
  // TODO
}; // class Graph

} // namespace IO
} // namespace Persistency

#endif
