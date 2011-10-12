/*
 * HandleIndirection.hpp
 *
 */
#ifndef INCLUDE_DATAFACADES_STRACCESS_HANDLEINDIRECTION_HPP_FILE
#define INCLUDE_DATAFACADES_STRACCESS_HANDLEINDIRECTION_HPP_FILE

/* public header */

#include <string>
#include <cassert>
#include <boost/mpl/if.hpp>

#include "System/NoInstance.hpp"
#include "DataFacades/StrAccess/SpecialMapKeys.hpp"
#include "DataFacades/StrAccess/IsPointer.hpp"
#include "DataFacades/StrAccess/IsSmartPointer.hpp"

namespace Persistency
{
namespace Facades
{
namespace StrAccess
{

namespace detail
{

/** \brief pointer dereferencing facility
 */
template<typename TFuncObj>
struct ProcessPointerNotNULL: private System::NoInstance
{
  /** \brief processing method.
   *  \param e element to be processed.
   *  \param p params to be used when processing.
   *  \return value farwarded from further user's calls.
   *  \warning this is internal - e must NOT be NULL!
   */
  template<typename T, typename TParams>
  static bool process(const T *e, TParams p)
  {
    assert(e!=NULL);
    return TFuncObj::process(*e, p);
  }

  /** \brief processing method for const char*.
   *  \param e element to be processed.
   *  \param p params to be used when processing.
   *  \return value farwarded from further user's calls.
   *  \warning this is internal - e must NOT be NULL!
   */
  template<typename TParams>
  static bool process(const char *e, TParams p)
  {
    assert(e!=NULL);
    // by making string() out of const char * we nesure no more pointer processing will be done
    return TFuncObj::process(std::string(e), p);
  }
};


/** \brief raw pointer extracting facility.
 */
template<typename TFuncObj>
struct StripPointer: private System::NoInstance
{
  /** \brief processing method.
   *  \param e element to be processed.
   *  \param p params to be used when processing.
   *  \return value farwarded from further user's calls.
   */
  template<typename T, typename TParams>
  static bool process(const T *e, TParams p)
  {
    if(e==NULL)
      return p.callback().nullOnPath(p.get());
    // process pointer further, ensuring that terms will be treated specially
    return ProcessPointerNotNULL<TFuncObj>::process(e, p);
  }
};


/** \brief smart pointer extracting facility.
 */
template<typename TFuncObj>
struct StripSmartPointer: private System::NoInstance
{
  /** \brief processing method.
   *  \param e element to be processed.
   *  \param p params to be used when processing.
   *  \return value farwarded from further user's calls.
   */
  template<typename T, typename TParams>
  static bool process(const T &e, TParams p)
  {
    // NOTE: TFuncObj is HandleIndirectionImpl<>, so pointer will be processed again
    return TFuncObj::process(e.get(), p);
  }
};


/** \brief facility that does nothing except of direct forwarding of a given result.
 */
template<typename TFuncObj>
struct NothingToStrip: private System::NoInstance
{
  /** \brief processing method.
   *  \param e element to be processed.
   *  \param p params to be used when processing.
   *  \return value farwarded from further user's calls.
   */
  template<typename T, typename TParams>
  static bool process(const T &e, TParams p)
  {
    return TFuncObj::process(e, p);
  }
};


/** \brief implementation class for indirection handling.
 */
template<typename TFuncObj>
struct HandleIndirectionImpl: private System::NoInstance
{
  /** \brief processing method.
   *  \param e element to be processed.
   *  \param p params to be used when processing.
   *  \return value farwarded from further user's calls.
   */
  template<typename T, typename TParams>
  static bool process(const T &e, TParams p)
  {
    typedef typename boost::mpl::if_c<IsPointer<T>::value,
                                 // then
                                      StripPointer< HandleIndirectionImpl<TFuncObj> >,
                                 // else
                                      NothingToStrip<TFuncObj>
                                 >::type
                                 IfStripPointer;

    typedef typename boost::mpl::if_c<IsSmartPointer<T>::value,
                                 // then
                                      StripSmartPointer< HandleIndirectionImpl<TFuncObj> >,
                                 // else
                                      IfStripPointer
                                 >::type
                                 IfStripSmartPointer;

    return IfStripSmartPointer::process(e,p);
  }
}; // struct HandleIndirectionImpl

} // namespace detail


/** \brief class handling indirection (pointers and smart pointers) in calls.
 *
 *  by using this facility it is ensured that element will be passed to the
 *  next processing stage (represented by TFuncObj template paramter to
 *  process() call) as dereferenced pointer (i.e. value), disregarding how
 *  many levels of nesting of pointers and smart pointer are introduced.
 *
 *  this wrapper is also smart enough to process terms (like const char *)
 *  in the secure whay, by translatinv them to std::string, if needed.
 */
struct HandleIndirection: private System::NoInstance
{
  /** \brief processing method.
   *  \param e element to be processed.
   *  \param p params to be used when processing.
   *  \return value farwarded from further user's calls.
   */
  template<typename TFuncObj, typename T, typename TParams>
  static bool process(const T &e, TParams p)
  {
    typedef typename TParams::template GetHandle<ErrorHandle>::type ErrH;
    ErrH::throwOnEnd(SYSTEM_SAVE_LOCATION, p);
    return detail::HandleIndirectionImpl<TFuncObj>::process(e,p);
  }
}; // struct HandleIndirection

} // namespace StrAccess
} // namespace Facades
} // namespace Persistency

#endif
