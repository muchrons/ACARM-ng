/*
 * HandleStopSignals.hpp
 *
 */
#ifndef INCLUDE_CORE_HANDLESTOPSIGNALS_HPP_FILE
#define INCLUDE_CORE_HANDLESTOPSIGNALS_HPP_FILE

/* public header */

#include "Core/SignalToStop.hpp"

namespace Core
{

/** \brief class setting up handlers for all requested signals.
 */
class HandleStopSignals
{
public:
  /** \brief create handlers for all signals.
   *  \param wt element ot be stopped uppon signal arrival. can be NULL.
   */
  explicit HandleStopSignals(WorkThreads *wt);

private:
  SignalToStop sigINT_;
  SignalToStop sigTERM_;
  SignalToStop sigPIPE_;
}; // class HandleStopSignals

} // namespace Core

#endif
