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

/** \brief helper object that checks if applicaiton's last run went fine.
 */
class CleanShutdownChecker: private boost::noncopyable
{
public:
  /** \brief check last run and create guard object.
   */
  CleanShutdownChecker(void);
  /** \brief perform marking on shutdown.
   */
  ~CleanShutdownChecker(void);

  /** \brief mark this run as clean.
   */
  void confirmCleanShutdown(void);

  /** \brief checks if last lunch finished successfull.
   *  \return true if last lunch was successful, false otherwise.
   */
  bool wasLastRunSuccessful(void) const;

private:
  Logger::Node log_;
  bool         lastRunSuccessful_;
  bool         confirmed_;
}; // class CleanShutdownChecker

} // namespace Core

#endif
