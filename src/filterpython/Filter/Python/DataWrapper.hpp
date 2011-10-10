/*
 * DataWrapper.hpp
 *
 */
#ifndef INCLUDE_FILTER_PYTHON_DATAWRAPPER_HPP_FILE
#define INCLUDE_FILTER_PYTHON_DATAWRAPPER_HPP_FILE

#include "PythonAPI/Python.hpp"
#include "Filter/Python/Data.hpp"

namespace Filter
{
namespace Python
{

/** \brief wrapper for python code.
 */
struct DataWrapper: public Data,
                    public boost::python::wrapper<Data>
{
}; // struct DataWrapper

} // namespace Python
} // namespace Filter

#endif
