/*
 * TriggerBaseWrapper.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_PYTHON_TRIGGERBASEWRAPPER_HPP_FILE
#define INCLUDE_TRIGGER_PYTHON_TRIGGERBASEWRAPPER_HPP_FILE

#include "PythonAPI/Python.hpp"
#include "Trigger/Python/TriggerBase.hpp"
#include "Trigger/Python/ExceptionNoImplementation.hpp"

namespace Trigger
{
namespace Python
{

/** \brief wrapper for python code.
 */
class TriggerBaseWrapper: public TriggerBase,
                   public boost::python::wrapper<TriggerBase>
{
private:
  virtual void triggerImpl(PythonAPI::Persistency::MetaAlert ma);
}; // class TriggerBaseWrapper

} // namespace Python
} // namespace Trigger

#endif
