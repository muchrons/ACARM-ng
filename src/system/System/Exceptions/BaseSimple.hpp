/*
 * BaseSimple.hpp
 */
#ifndef INCLUDE_SYSTEM_EXCEPTIONS_BASESIMPLE_HPP_FILE
#define INCLUDE_SYSTEM_EXCEPTIONS_BASESIMPLE_HPP_FILE

/* public header */

#include <string>
#include <stdexcept>

namespace System
{
namespace Exceptions
{

/** \brief generic base for exceptions implementation.
 *  \note this class does not provide default implementations for standard
 *        exception methods therefor it requires to have them already
 *        defined in StdBase (as in std::logic_error for instance).
 *  \param CRTP    derived class (to start new hierarchy.
 *  \param StdBase base class for exception from sntadard library.
 */
template<typename CRTP, typename StdBase>
class BaseSimple: public StdBase
{
public:
  /** \brief return error message as string.
   *  \return error message.
   */
  inline std::string whatAsStr(void) const
  {
    return StdBase::what();
  }

protected:
  /** \brief create execption with given message.
   *  \param msg message to represent.
   */
  template<typename T>
  explicit BaseSimple(const T &msg):
    StdBase(msg)
  {
  }
}; // class BaseSimple

}; // namespace Exceptions
}; // namespace System

#endif
