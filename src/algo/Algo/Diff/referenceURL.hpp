/*
 * referenceURL.hpp
 *
 */
#ifndef INCLUDE_ALGO_DIFF_REFERENCEURL_HPP_FILE
#define INCLUDE_ALGO_DIFF_REFERENCEURL_HPP_FILE

/* public header */

#include "Persistency/ReferenceURL.hpp"
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
Similarity compare(const Persistency::ReferenceURL &e1, const Persistency::ReferenceURL &e2);

} // namespace Diff
} // namespace Algo

#endif
