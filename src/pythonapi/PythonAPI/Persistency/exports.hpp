/*
 * exports.hpp
 *
 */
#ifndef INCLUDE_PYTHONAPI_PERSISTENCY_EXPORTS_HPP_FILE
#define INCLUDE_PYTHONAPI_PERSISTENCY_EXPORTS_HPP_FILE

#include "PythonAPI/Python.hpp"
#include "PythonAPI/Persistency/exportCertainty.hpp"
#include "PythonAPI/Persistency/exportTimestamp.hpp"

BOOST_PYTHON_MODULE(persistency)
{
  PythonAPI::Persistency::exportCertainty();
  PythonAPI::Persistency::exportTimestamp();
}

#endif
