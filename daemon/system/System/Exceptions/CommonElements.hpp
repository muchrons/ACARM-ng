/*
 * CommonElements.hpp
 *
 */
#ifndef INCLUDE_SYSTEM_EXCEPTIONS_COMMONELEMENTS_HPP_FILE
#define INCLUDE_SYSTEM_EXCEPTIONS_COMMONELEMENTS_HPP_FILE

/* public header */

#include <string>
#include <sstream>
#include <typeinfo>

#include "System/Exceptions/detail/Location.hpp"
#include "System/FunctionName.hpp"

namespace System
{
namespace Exceptions
{

/** \brief macro for saving location in file.
 */
#define SYSTEM_SAVE_LOCATION ::System::Exceptions::CommonElements::Location(__FILE__, __LINE__, SYSTEM_FUNCTION_NAME)


/** \brief common elements used by exceptions.
 */
struct CommonElements
{
  /** \brief gets type name by means of RTTI.
   *  \param t class to check.
   *  \return name of class.
   */
  template<typename T>
  std::string getTypeName(const T &t) const
  {
    return typeid(t).name();
  }

  /** \brief location class name to be used in user's code. */
  typedef detail::Location Location;

  /** \brief create string from concatenating arguments.
   *  \param t1 any paramter.
   *  \param t2 any paramter.
   *  \return concatenated string.
   */
  template<typename T1, typename T2>
  std::string cc(const T1& t1, const T2 &t2) const
  {
    std::stringstream ss;
    ss<<t1<<t2;
    return ss.str();
  }
  /** \brief create string from concatenating arguments.
   *  \param t1 any paramter.
   *  \param t2 any paramter.
   *  \param t3 any paramter.
   *  \return concatenated string.
   */
  template<typename T1, typename T2, typename T3>
  std::string cc(const T1& t1, const T2 &t2, const T3 &t3) const
  {
    std::stringstream ss;
    ss<<t1<<t2<<t3;
    return ss.str();
  }
  /** \brief create string from concatenating arguments.
   *  \param t1 any paramter.
   *  \param t2 any paramter.
   *  \param t3 any paramter.
   *  \param t4 any paramter.
   *  \return concatenated string.
   */
  template<typename T1, typename T2, typename T3, typename T4>
  std::string cc(const T1& t1, const T2 &t2, const T3 &t3, const T4 &t4) const
  {
    std::stringstream ss;
    ss<<t1<<t2<<t3<<t4;
    return ss.str();
  }
  /** \brief create string from concatenating arguments.
   *  \param t1 any paramter.
   *  \param t2 any paramter.
   *  \param t3 any paramter.
   *  \param t4 any paramter.
   *  \param t5 any paramter.
   *  \return concatenated string.
   */
  template<typename T1, typename T2, typename T3, typename T4, typename T5>
  std::string cc(const T1& t1, const T2 &t2, const T3 &t3, const T4 &t4, const T5& t5) const
  {
    std::stringstream ss;
    ss<<t1<<t2<<t3<<t4<<t5;
    return ss.str();
  }
  /** \brief create string from concatenating arguments.
   *  \param t1 any paramter.
   *  \param t2 any paramter.
   *  \param t3 any paramter.
   *  \param t4 any paramter.
   *  \param t5 any paramter.
   *  \param t6 any paramter.
   *  \return concatenated string.
   */
  template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
  std::string cc(const T1& t1, const T2 &t2, const T3 &t3, const T4 &t4, const T5& t5, const T6 &t6) const
  {
    std::stringstream ss;
    ss<<t1<<t2<<t3<<t4<<t5<<t6;
    return ss.str();
  }
}; // class CommonElements

} // namespace Exceptions
} // namespace System

#endif
