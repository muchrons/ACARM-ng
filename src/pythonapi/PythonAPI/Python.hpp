/*
 * Python.hpp
 *
 */
#ifndef INCLUDE_PYTHONAPI_PYTHON_HPP_FILE
#define INCLUDE_PYTHONAPI_PYTHON_HPP_FILE

/* public header */

// NOTE: some system libraries define macros _POSIX_C_SOURCE and _XOPEN_SOURCE if
//       user didn't define it already. for example _POSIX_C_SOURCE is required
//       to be defined by user in order to be POSIX-compliant and if it is not
//       glibc does this in some of the header files.
//       Python people however define these macros by themselves, without asking
//       and without event checking if they are already defined.
//       what you see here is a nasty hack to work around brain-deadness of PythonC
//       authors who apparently does not understand the difference between
//       a generic library and an application:
//       http://bugs.python.org/issue793764
//       and refuse to change this behavior
//       http://docs.python.org/c-api/intro.html#includes
//       even though it is obviously wrong (if you think otherwise think what would
//       it be like if every library took the same approach).
//       any way hack's nasty. don't do it at home!
//       ps
//       this is probably the worst hack i've done in years... oh gods! spare my soul!

#ifdef _POSIX_C_SOURCE
#undef _POSIX_C_SOURCE
#endif

#ifdef _XOPEN_SOURCE
#undef _XOPEN_SOURCE
#endif

#ifdef _XOPEN_SOURCE_EXTENDED
#undef _XOPEN_SOURCE_EXTENDED
#endif

#ifdef __BSD_VISIBLE
#undef __BSD_VISIBLE
#endif

// NOTE: we are wrapping around boost::python, so we include these headers directly
//       though the above mentioned hack is required for PythonC not the boost::python.
#include <boost/python.hpp>
#include <boost/python/module.hpp>

#endif
