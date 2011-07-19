/*
 * exports.hpp
 *
 */
#ifndef INCLUDE_PYTHONAPI_PERSISTENCY_EXPORTS_HPP_FILE
#define INCLUDE_PYTHONAPI_PERSISTENCY_EXPORTS_HPP_FILE

#include "PythonAPI/Python.hpp"
#include "PythonAPI/Persistency/exportCertainty.hpp"
#include "PythonAPI/Persistency/exportTimestamp.hpp"
#include "PythonAPI/Persistency/exportMD5Sum.hpp"
#include "PythonAPI/Persistency/exportPortNumber.hpp"
#include "PythonAPI/Persistency/exportSeverityLevel.hpp"

BOOST_PYTHON_MODULE(persistency)
{
  PythonAPI::Persistency::exportCertainty();
  PythonAPI::Persistency::exportTimestamp();
  PythonAPI::Persistency::exportMD5Sum();
  PythonAPI::Persistency::exportPortNumber();
  PythonAPI::Persistency::exportSeverityLevel();
}

#endif
