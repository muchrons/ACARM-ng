/*
 * Certanity.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_CERTANITY_HPP_FILE
#define INCLUDE_PERSISTENCY_CERTANITY_HPP_FILE

/* public header */

#include <cassert>

#include "Persistency/Exception.hpp"


namespace Persistency
{

/** \brief exception on invalid certanity level.
 */
struct ExceptionInvalidCertanity: public Exception
{
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   */
  ExceptionInvalidCertanity(const char *where):
    Exception(where, "certanity out of bound requested")
  {
  }
}; // struct ExceptionInvalidCertanity


/** \brief certanity level representation.
 */
class Certanity
{
public:
  /** \brief create certanity representation.
   *  \param c certanity value.
   *  \note c-tor is not explicit to make usage simpler.
   */
  Certanity(const double c=100):
    c_(c)
  {
    if(c_<=0 || 100<c_)
      throw ExceptionInvalidCertanity(CALLNAME);
  }
  /** \brief gets certanity level.
   *  \return certanity saved in class.
   */
  double get(void) const
  {
    assert(0<c_);
    assert(c_<=100);
    return c_;
  }

private:
  double c_;
}; // class Certanity

} // namespace Presistency

#endif
