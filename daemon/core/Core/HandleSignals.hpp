/*
 * HandleStopSignals.hpp
 *
 */
#ifndef INCLUDE_CORE_HANDLESIGNALS_HPP_FILE
#define INCLUDE_CORE_HANDLESIGNALS_HPP_FILE

/* public header */

#include "Core/SignalToReconfigure.hpp"
#include "Core/SignalToStop.hpp"
#include "Core/IgnoreSignal.hpp"

namespace Core
{

/** \brief class setting up handles for all requested signals.
 */
class HandleSignals
{
public:
  /** \brief create handles for all signals.
   *  \param wt element ot be stopped uppon signal arrival. can be NULL.
   */
  explicit HandleSignals(WorkThreads *wt);

private:
  SignalToReconfigure sigHUP_;
  SignalToStop        sigINT_;
  SignalToStop        sigTERM_;
  IgnoreSignal        sigPIPE_;
}; // class HandleSignals

} // namespace Core

#endif
