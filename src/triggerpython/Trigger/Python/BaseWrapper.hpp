/*
 * BaseWrapper.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_PYTHON_BASEWRAPPER_HPP_FILE
#define INCLUDE_TRIGGER_PYTHON_BASEWRAPPER_HPP_FILE

#include "PythonAPI/Python.hpp"
#include "Trigger/Python/Base.hpp"
#include "Trigger/Python/ExceptionNoImplementation.hpp"

namespace Trigger
{
namespace Python
{

/** \brief wrapper for python code.
 */
class BaseWrapper: public Base,
                   public boost::python::wrapper<Base>
{
public:
  BaseWrapper(void);
  /*
  explicit BaseWrapper(const boost::reference_wrapper<const Trigger::Python::BaseWrapper>::type &other);
  */

private:
  virtual void triggerImpl(PythonAPI::Persistency::MetaAlert ma);
}; // class BaseWrapper

} // namespace Python
} // namespace Trigger

#endif
