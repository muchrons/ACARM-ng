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

  /** \brief call checking if given entry is interesting.
   *  \param thisEntry entry to be checked.
   *  \param data      data associated with this object.
   *  \return true if entry is interesting, false otherwise.
   */
  bool isEntryInteresting(Persistency::ConstGraphNodePtrNN thisEntry, DataPtr data) const;

  /** \brief call getting meta-alert's name from the user's implementation.
   *  \param thisEntry      entry created during this run (incomming meta-alert).
   *  \param thisEntryData  data assigned to thisEntry.
   *  \param otherEntry     entry it has been correlated with.
   *  \param otherEntryData data assigned to otherEntry.
   *  \return name for the new meta-alert.
   */
  Persistency::MetaAlert::Name getMetaAlertName(Persistency::ConstGraphNodePtrNN thisEntry,
                                                DataPtr                          thisEntryData,
                                                Persistency::ConstGraphNodePtrNN otherEntry,
                                                DataPtr                          otherEntryData) const;

  /** \brief call checking if given two meta-alerts can be correlated together.
   *  \param thisEntry      entry created during this run (incomming meta-alert).
   *  \param thisEntryData  data assigned to thisEntry.
   *  \param otherEntry     entry it has been correlated with.
   *  \param otherEntryData data assigned to otherEntry.
   *  \return true, if entries can be correlated, false otherwise.
   */
  bool canCorrelate(Persistency::ConstGraphNodePtrNN thisEntry,
                    DataPtr                          thisEntryData,
                    Persistency::ConstGraphNodePtrNN otherEntry,
                    DataPtr                          otherEntryData) const;

  /** \brief create data entry for a given, incomming node.
   *  \param n node to create data for.
   *  \return created data, or NULL.
   */
  DataPtr makeThisEntryUserData(Persistency::ConstGraphNodePtrNN n) const;

  /** \brief call creating user's data for a given, new node.
   *  \param thisEntry      entry created during this run (incomming meta-alert).
   *  \param thisEntryData  data assigned to thisEntry.
   *  \param otherEntry     entry it has been correlated with.
   *  \param otherEntryData data assigned to otherEntry.
   *  \param newNode        newly created node (data for it is to be generated during this call).
   *  \return data assigned, or NULL.
   */
  DataPtr makeUserDataForNewNode(Persistency::ConstGraphNodePtrNN thisEntry,
                                 DataPtr                          thisEntryData,
                                 Persistency::ConstGraphNodePtrNN otherEntry,
                                 DataPtr                          otherEntryData,
                                 Persistency::ConstGraphNodePtrNN newNode) const;

protected:
  /** \brief meta-alert access facede (short name). */
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
  virtual DataPtr makeThisEntryUserDataImpl(PyMetaAlert n) const = 0;
  virtual DataPtr makeUserDataForNewNodeImpl(PyMetaAlert thisEntry,
                                             DataPtr     thisEntryData,
                                             PyMetaAlert otherEntry,
                                             DataPtr     otherEntryData,
                                             PyMetaAlert newNode) const = 0;
}; // class Base


typedef boost::shared_ptr<Base>         BasePtr;
typedef Commons::SharedPtrNotNULL<Base> BasePtrNN;

} // namespace Python
} // namespace Filter

#endif
