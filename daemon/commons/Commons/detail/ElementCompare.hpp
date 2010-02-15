/*
 * ElementCompare.hpp
 *
 */
#ifndef INCLUDE_COMMONS_DETAIL_ELEMENTCOMPARE_HPP_FILE
#define INCLUDE_COMMONS_DETAIL_ELEMENTCOMPARE_HPP_FILE

/* public header */

#include <boost/shared_ptr.hpp>

#include "Commons/SharedPtrNotNULL.hpp"

namespace Commons
{
//
// following declarations may look strange, but partial specializations of
// functions nor methods are not allowed, and so proper classes had to be
// created to handle this.
//
namespace detail
{

/** \brief helper for values comparison.
 */
template<typename T>
struct ElementCompare
{
  /** \brief compare elements.
   *  \param e1 first element.
   *  \param e2 second element.
   *  \return true, if elements are equal, false otherwise.
   */
  static bool equal(const T &e1, const T &e2)
  {
    return e1==e2;
  }
}; // struct ElementCompare


/** \brief helper for pointers comparison.
 */
template<typename T>
struct ElementCompare<T*>
{
  /** \brief compare elements.
   *  \param e1 first element.
   *  \param e2 second element.
   *  \return true, if elements are equal, false otherwise.
   */
  static bool equal(const T *e1, const T *e2)
  {
    return Base::ViaPointer::equal(e1, e2);
  }
}; // struct ElementCompare


/** \brief helper for boost::share_ptr<> comparison.
 */
template<typename T>
struct ElementCompare< boost::shared_ptr<T> >
{
  /** \brief compare elements.
   *  \param e1 first element.
   *  \param e2 second element.
   *  \return true, if elements are equal, false otherwise.
   */
  static bool equal(const boost::shared_ptr<T> e1,
                    const boost::shared_ptr<T> e2)
  {
    return ElementCompare<T*>::equal( e1.get(), e2.get() );
  }
}; // struct ElementCompare


/** \brief helper for SharedPtrNotNULL<> comparison.
 */
template<typename T>
struct ElementCompare< SharedPtrNotNULL<T> >
{
  /** \brief compare elements.
   *  \param e1 first element.
   *  \param e2 second element.
   *  \return true, if elements are equal, false otherwise.
   */
  static bool equal(const SharedPtrNotNULL<T> e1,
                    const SharedPtrNotNULL<T> e2)
  {
    return ElementCompare<T*>::equal( e1.get(), e2.get() );
  }
}; // struct ElementCompare

} // namespace detail
} // namespace Commons

#endif
