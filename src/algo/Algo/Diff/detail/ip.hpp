/*
 * ip.hpp
 *
 */
#ifndef INCLUDE_ALGO_DIFF_DETAIL_IP_HPP_FILE
#define INCLUDE_ALGO_DIFF_DETAIL_IP_HPP_FILE

/* public header */

#include <boost/asio/ip/address.hpp>

#include "Algo/Diff/detail/Comparer.hpp"

namespace Algo
{
namespace Diff
{
namespace detail
{

/** \brief specialization for IP.
 */
template<>
struct Comparer<const boost::asio::ip::address>
{
  /** \brief compares two elements.
   *  \param e1 first element to compare.
   *  \param e2 second element to compare.
   *  \return result of the comparison.
   */
  static Similarity cmp(const boost::asio::ip::address &e1, const boost::asio::ip::address &e2);
}; // struct Comparer<const boost::asio::ip::address>

} // namespace detail
} // namespace Diff
} // namespace Algo

#endif
