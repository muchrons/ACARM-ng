/*
 * Base.hpp
 *
 */
#ifndef INCLUDE_FILTER_PYTHON_BASE_HPP_FILE
#define INCLUDE_FILTER_PYTHON_BASE_HPP_FILE

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

#include "Logger/Logger.hpp"
#include "Commons/SharedPtrNotNULL.hpp"
#include "Persistency/GraphNode.hpp"
#include "PythonAPI/Persistency/MetaAlert.hpp"
#include "Filter/Python/Data.hpp"
#include "Filter/Python/ExceptionNoImplementation.hpp"

namespace Filter
{
namespace Python
{

/** \brief base interface for strategy to operate on.
 */
class Base: private boost::noncopyable
{
public:
  /** \brief ensure secure, polymorphic destruction.
   */
  virtual ~Base(void);

  /** \brief call hecking if given entry is interesting.
   *  \param thisEntry entry to be checked.
   *  \param data      data associated with this object.
   *  \return true if entry is interesting, false otherwise.
   */
  bool isEntryInteresting(Persistency::ConstGraphNodePtrNN thisEntry, DataPtr data) const;

  Persistency::MetaAlert::Name getMetaAlertName(Persistency::ConstGraphNodePtrNN thisEntry,
                                                DataPtr                          thisEntryData,
                                                Persistency::ConstGraphNodePtrNN otherEntry,
                                                DataPtr                          otherEntryData) const;

  bool canCorrelate(Persistency::ConstGraphNodePtrNN thisEntry,
                    DataPtr                          thisEntryData,
                    Persistency::ConstGraphNodePtrNN otherEntry,
                    DataPtr                          otherEntryData) const;

protected:
  typedef PythonAPI::Persistency::MetaAlert PyMetaAlert;

  /** \brief creates instance.
   */
  Base(void);

  const Logger::Node log_;  ///< logger to use

private:
  virtual bool isEntryInterestingImpl(PyMetaAlert thisEntry, DataPtr data) const = 0;
  virtual std::string getMetaAlertNameImpl(PyMetaAlert thisEntry,
                                           DataPtr     thisEntryData,
                                           PyMetaAlert otherEntry,
                                           DataPtr     otherEntryData) const = 0;
  virtual bool canCorrelateImpl(PyMetaAlert thisEntry,
                                DataPtr     thisEntryData,
                                PyMetaAlert otherEntry,
                                DataPtr     otherEntryData) const = 0;
}; // class Base


typedef boost::shared_ptr<Base>         BasePtr;
typedef Commons::SharedPtrNotNULL<Base> BasePtrNN;

} // namespace Python
} // namespace Filter

#endif
