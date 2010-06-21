/*
 * HandleStopSignals.hpp
 *
 */
#ifndef INCLUDE_CORE_HANDLESIGNALS_HPP_FILE
#define INCLUDE_CORE_HANDLESIGNALS_HPP_FILE

/* public header */

#include "Core/SignalToStop.hpp"
#include "Core/IgnoreSignal.hpp"

namespace Core
{

/** \brief class setting up handlers for all requested signals.
 */
class HandleSignals
{
public:
  /** \brief create handlers for all signals.
   *  \param wt element ot be stopped uppon signal arrival. can be NULL.
   */
  explicit HandleSignals(WorkThreads *wt);

private:
  SignalToStop sigINT_;
  SignalToStop sigTERM_;
  IgnoreSignal sigPIPE_;
  IgnoreSignal sigHUP_;
}; // class HandleSignals

} // namespace Core

#endif
