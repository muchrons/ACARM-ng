/*
 * CUrlInit.hpp
 *
 */
#ifndef INCLUDE_FILTER_IPBLACKLIST_CURLINIT_HPP_FILE
#define INCLUDE_FILTER_IPBLACKLIST_CURLINIT_HPP_FILE

#include "Logger/Node.hpp"

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

private:
  Logger::Node log_;
}; // class CUrlInit

} // namespace IPBlackList
} // namespace Filter

#endif
