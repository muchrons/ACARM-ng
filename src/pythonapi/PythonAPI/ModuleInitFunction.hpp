/*
 * ModuleInitFunction.hpp
 *
 */
#ifndef INCLUDE_PYTHONAPI_MODULEINITFUNCTION_HPP_FILE
#define INCLUDE_PYTHONAPI_MODULEINITFUNCTION_HPP_FILE

/* public header */

namespace PythonAPI
{

/** \brief helper typedef for initialization function. */
typedef void (*ModuleInitFunction)(void);
//typedef PyObject* (*ModuleInitFunction)(void);

} // namespace PythonAPI

#endif
