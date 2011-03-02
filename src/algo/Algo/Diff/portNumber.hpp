/*
 * portNumber.hpp
 *
 */
#ifndef INCLUDE_ALGO_DIFF_PORTNUMBER_HPP_FILE
#define INCLUDE_ALGO_DIFF_PORTNUMBER_HPP_FILE

/* public header */

#include "Persistency/PortNumber.hpp"
#include "Algo/Diff/Similarity.hpp"

namespace Algo
{
namespace Diff
{

/** \brief compares two elements.
 *  \param e1 first element to compare.
 *  \param e2 second element to compare.
 *  \return result of the comparison.
 */
inline Similarity compare(const Persistency::PortNumber e1, const Persistency::PortNumber e2)
{
  return ( e1.get()==e2.get() )?1:0;
} // compare()

} // namespace Diff
} // namespace Algo

#endif
