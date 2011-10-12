/*
 * OnMetaAlert.hpp
 *
 */
#ifndef INCLUDE_DATAFACADES_STRACCESS_ONGRAPHNODE_HPP_FILE
#define INCLUDE_DATAFACADES_STRACCESS_ONGRAPHNODE_HPP_FILE

/* public header */

#include <vector>

#include "System/NoInstance.hpp"
#include "Persistency/GraphNode.hpp"
#include "DataFacades/StrAccess/MainDispatcher.hpp"

namespace DataFacades
{
namespace StrAccess
{

/** \brief handle processing of graph node objects.
 *
 *  note that there is a little tricky naming here. class is names on meta alert
 *  but handles graph node objects. this is due to the fact that graph node
 *  objects represent meta alerts in the code, while meta alert class just
 *  groups meta-alert related fields together.
 */
struct OnMetaAlert: private System::NoInstance
{
  /** \brief processing method.
   *  \param e element to be processed.
   *  \param p params to be used when processing.
   *  \return value farwarded from further user's calls.
   */
  template<typename TParams>
  static bool process(const Persistency::GraphNode &e, TParams p)
  {
    typedef typename TParams::template GetHandle<ErrorHandle>::type ErrH;

    ErrH::throwOnEnd(SYSTEM_SAVE_LOCATION, p);
    ErrH::throwOnLast(SYSTEM_SAVE_LOCATION, p);
    ErrH::throwOnInvalidName(SYSTEM_SAVE_LOCATION, p, "metaalert");

    ++p;

    if(p.get()=="name")
      return MainDispatcher::process(e.getMetaAlert()->getName().get(), p);
    if(p.get()=="id")
      return MainDispatcher::process(e.getMetaAlert()->getID().get(), p);
    if(p.get()=="severitydelta")
      return MainDispatcher::process(e.getMetaAlert()->getSeverityDelta(), p);
    if(p.get()=="certaintydelta")
      return MainDispatcher::process(e.getMetaAlert()->getCertaintyDelta(), p);
    if(p.get()=="referenceurl")
      return MainDispatcher::process(e.getMetaAlert()->getReferenceURL(), p);
    if(p.get()=="created")
      return MainDispatcher::process(e.getMetaAlert()->getCreateTime().get(), p);
    if(p.get()=="isleaf")
      return MainDispatcher::process(e.isLeaf(), p);
    if(p.get()=="alert")
      return MainDispatcher::process(e.getAlert(), p);
    if(p.get()=="children")
      return MainDispatcher::process( getAllChildren(e), p);
    if(p.get()=="leafs")
      return MainDispatcher::process( getAllLeafs(e), p);

    ErrH::throwOnInvalidPath(SYSTEM_SAVE_LOCATION, p);
    return false;
  }

private:
  typedef std::vector<Persistency::GraphNodePtrNN> Nodes;

  static Nodes getAllChildren(const Persistency::GraphNode &root);
  static Nodes getAllLeafs(const Persistency::GraphNode &root);
}; // struct OnMetaAlert

} // namespace StrAccess
} // namespace DataFacades

#endif
