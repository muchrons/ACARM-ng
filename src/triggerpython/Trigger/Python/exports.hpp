/*
 * exports.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_PYTHON_EXPORTS_HPP_FILE
#define INCLUDE_TRIGGER_PYTHON_EXPORTS_HPP_FILE

#include "PythonAPI/Python.hpp"
#include "Trigger/Python/exportBaseWrapper.hpp"

BOOST_PYTHON_MODULE(trigger)
{
  Trigger::Python::exportBaseWrapper();
}

#endif
