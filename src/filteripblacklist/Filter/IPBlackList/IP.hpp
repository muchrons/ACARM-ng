/*
 * IP.hpp
 *
 */
#ifndef INCLUDE_FILTER_IPBLACKLIST_IP_HPP_FILE
#define INCLUDE_FILTER_IPBLACKLIST_IP_HPP_FILE

#include "Persistency/IPTypes.hpp"

namespace Filter
{
namespace IPBlackList
{
namespace detail
{
/** \brief helper class for IPTypes paramter. */
struct Nothing
{
}; // struct Nothing
} // namespace detail

/** \brief declaration of IP. */
typedef Persistency::IPTypes<detail::Nothing>::IP IP;

} // namespace IPBlackList
} // namespace Filter

#endif
