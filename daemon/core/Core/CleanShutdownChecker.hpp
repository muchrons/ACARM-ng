/*
 * CleanShutdownChecker.hpp
 *
 */
#ifndef INCLUDE_CORE_CLEANSHUTDOWNCHECKER_HPP_FILE
#define INCLUDE_CORE_CLEANSHUTDOWNCHECKER_HPP_FILE

/* public header */

#include <boost/noncopyable.hpp>

#include "Logger/Node.hpp"

namespace Core
{

/** \brief 
 */
class CleanShutdownChecker: private boost::noncopyable
{
public:
  /** \brief 
   */
  CleanShutdownChecker(void);
  /** \brief 
   */
  ~CleanShutdownChecker(void);

  /** \brief 
   */
  void confirmCleanShutdown(void);

private:
  Logger::Node log_;
  bool         confirmed_;
}; // class CleanShutdownChecker

} // namespace Core

#endif
