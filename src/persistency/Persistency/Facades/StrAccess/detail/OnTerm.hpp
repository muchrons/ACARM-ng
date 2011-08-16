/*
 * OnTerm.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_FACADES_STRACCESS_DETAIL_ONTERM_HPP_FILE
#define INCLUDE_PERSISTENCY_FACADES_STRACCESS_DETAIL_ONTERM_HPP_FILE

/* public header */

#include <string>
#include <cstdlib>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include "System/NoInstance.hpp"
#include "Commons/Convert.hpp"
#include "Persistency/MD5Sum.hpp"
#include "Persistency/IPTypes.hpp"
#include "Persistency/Facades/StrAccess/IsTerm.hpp"
#include "Persistency/Facades/StrAccess/SpecialMapKeys.hpp"
#include "Persistency/Facades/StrAccess/collectionSize.hpp"

namespace Persistency
{
namespace Facades
{
namespace StrAccess
{
namespace detail
{

/** \brief implementation of term element.
 */
struct OnTermImpl: private System::NoInstance,
                   public  IPTypes<OnTermImpl>
{
  /** \brief processing method.
   *  \param e element to be processed.
   *  \param p params to be used when processing.
   *  \return value farwarded from further user's calls.
   */
  template<typename T, typename TParams>
  static bool process(const T &e, TParams &p)
  {
    return p.callback().value( Commons::Convert::to<std::string>(e) );
  }

  /** \brief special method for handling MD5Sum as a term.
   *  \param e element to be processed.
   *  \param p params to be used when processing.
   *  \return value farwarded from further user's calls.
   */
  template<typename TParams>
  static bool process(const MD5Sum &e, TParams &p)
  {
    assert(e.get()!=NULL);
    return process(std::string(e.get()), p);
  }

  /** \brief special method for handling boost::asio::ip::address as a term.
   *  \param e element to be processed.
   *  \param p params to be used when processing.
   *  \return value farwarded from further user's calls.
   */
  template<typename TParams>
  static bool process(const IP &e, TParams &p)
  {
    BOOST_STATIC_ASSERT( (boost::is_same<IP,Netmask>::value) );
    return process(e.to_string(), p);
  }
  /** \brief special method for handling boost::asio::ip::address_v4 as a term.
   *  \param e element to be processed.
   *  \param p params to be used when processing.
   *  \return value farwarded from further user's calls.
   */
  template<typename TParams>
  static bool process(const IPv4 &e, TParams &p)
  {
    BOOST_STATIC_ASSERT( (boost::is_same<IPv4,Netmask_v4>::value) );
    return process(e.to_string(), p);
  }
  /** \brief special method for handling boost::asio::ip::address_v6 as a term.
   *  \param e element to be processed.
   *  \param p params to be used when processing.
   *  \return value farwarded from further user's calls.
   */
  template<typename TParams>
  static bool process(const IPv6 &e, TParams &p)
  {
    BOOST_STATIC_ASSERT( (boost::is_same<IPv6,Netmask_v6>::value) );
    return process(e.to_string(), p);
  }
  /** \brief special method for handling boolean values as a term.
   *  \param e element to be processed.
   *  \param p params to be used when processing.
   *  \return value farwarded from further user's calls.
   */
  template<typename TParams>
  static bool process(const bool e, TParams &p)
  {
    return process(std::string(e?"true":"false"), p);
  }
}; // struct OnTermImpl


/** \brief warpper used for handling term elements.
 */
struct OnTerm: private System::NoInstance
{
  /** \brief processing method.
   *  \param e element to be processed.
   *  \param p params to be used when processing.
   *  \return value farwarded from further user's calls.
   */
  template<typename T, typename TParams>
  static bool process(const T &e, TParams &p)
  {
    // sanity check
    typedef typename TParams::template GetHandle<ErrorHandle>::type ErrH;
    ErrH::throwIfEnd(SYSTEM_SAVE_LOCATION, p);
    ErrH::throwIfNotLast(SYSTEM_SAVE_LOCATION, p);
    BOOST_STATIC_ASSERT(IsTerm<T>::value);
    // process final type (term)
    return OnTermImpl::process(e, p);
  }
}; // struct OnTerm

} // namespace detail
} // namespace StrAccess
} // namespace Facades
} // namespace Persistency

#endif
