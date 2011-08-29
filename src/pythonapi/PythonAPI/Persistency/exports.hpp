/*
 * exports.hpp
 *
 */
#ifndef INCLUDE_PYTHONAPI_PERSISTENCY_EXPORTS_HPP_FILE
#define INCLUDE_PYTHONAPI_PERSISTENCY_EXPORTS_HPP_FILE

#include "PythonAPI/Python.hpp"
#include "PythonAPI/Persistency/exportOptionalString.hpp"
#include "PythonAPI/Persistency/exportMetaAlert.hpp"

BOOST_PYTHON_MODULE(persistency)
{
  PythonAPI::Persistency::exportOptionalString();
  PythonAPI::Persistency::exportMetaAlert();
}

#endif
