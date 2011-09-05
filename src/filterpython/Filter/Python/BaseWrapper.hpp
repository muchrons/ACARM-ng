/*
 * BaseWrapper.hpp
 *
 */
#ifndef INCLUDE_FILTER_PYTHON_BASEWRAPPER_HPP_FILE
#define INCLUDE_FILTER_PYTHON_BASEWRAPPER_HPP_FILE

#include "PythonAPI/Python.hpp"
#include "Filter/Python/Base.hpp"
#include "Filter/Python/ExceptionNoImplementation.hpp"

namespace Filter
{
namespace Python
{

/** \brief wrapper for python code.
 */
class BaseWrapper: public Base,
                   public boost::python::wrapper<Base>
{
private:
  //virtual void filterImpl(PythonAPI::Persistency::MetaAlert ma);
}; // class BaseWrapper

} // namespace Python
} // namespace Filter

#endif
