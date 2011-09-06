/*
 * FilterBaseWrapper.hpp
 *
 */
#ifndef INCLUDE_FILTER_PYTHON_FILTERBASEWRAPPER_HPP_FILE
#define INCLUDE_FILTER_PYTHON_FILTERBASEWRAPPER_HPP_FILE

#include "PythonAPI/Python.hpp"
#include "Filter/Python/FilterBase.hpp"
#include "Filter/Python/ExceptionNoImplementation.hpp"

namespace Filter
{
namespace Python
{

/** \brief wrapper for python code.
 */
class FilterBaseWrapper: public FilterBase,
                         public boost::python::wrapper<FilterBase>
{
private:
  virtual bool isEntryInterestingImpl(PyMetaAlert thisEntry, DataPtr data) const;
  virtual std::string getMetaAlertNameImpl(PyMetaAlert thisEntry,
                                           DataPtr     thisEntryData,
                                           PyMetaAlert otherEntry,
                                           DataPtr     otherEntryData) const;
  virtual bool canCorrelateImpl(PyMetaAlert thisEntry,
                                DataPtr     thisEntryData,
                                PyMetaAlert otherEntry,
                                DataPtr     otherEntryData) const;
  virtual DataPtr makeThisEntryUserDataImpl(PyMetaAlert n) const;
  virtual DataPtr makeUserDataForNewNodeImpl(PyMetaAlert thisEntry,
                                             DataPtr     thisEntryData,
                                             PyMetaAlert otherEntry,
                                             DataPtr     otherEntryData,
                                             PyMetaAlert newNode) const;
}; // class FilterBaseWrapper

} // namespace Python
} // namespace Filter

#endif
