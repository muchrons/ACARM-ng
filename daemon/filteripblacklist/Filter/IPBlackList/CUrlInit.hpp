/*
 * CUrlInit.hpp
 *
 */
#ifndef INCLUDE_FILTER_IPBLACKLIST_CURLINIT_HPP_FILE
#define INCLUDE_FILTER_IPBLACKLIST_CURLINIT_HPP_FILE

namespace Filter
{
namespace IPBlackList
{

/** \brief class for initialization of CUrl++.
 */
class CUrlInit
{
public:
  /** \brief initilize (if not already initialized).
   */
  CUrlInit(void);
  /** \brief mark as not used any more.
   */
  ~CUrlInit(void);
}; // class CUrlInit

} // namespace IPBlackList
} // namespace Filter

#endif
