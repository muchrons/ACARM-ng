/*
 * CUrlInit.hpp
 *
 */
#ifndef INCLUDE_FILTER_IPBLACKLIST_CURLINIT_HPP_FILE
#define INCLUDE_FILTER_IPBLACKLIST_CURLINIT_HPP_FILE

#include "System/GlobalInit.hpp"

namespace Filter
{
namespace IPBlackList
{
namespace detail
{
/** \brief implementation of CUrl++ init/uninit.
 */
struct CUrlInitImpl
{
  /** \brief initilize library.
   */
  static void init(void);
  /** \brief uninitialize library.
   */
  static void uninit(void);
}; // struct CUrlInitImpl
} // namespace detail


/** \brief class responsible for (re)initialization of CURL++ library.
 */
typedef System::GlobalInit<detail::CUrlInitImpl> CUrlInit;

} // namespace IPBlackList
} // namespace Filter

#endif
