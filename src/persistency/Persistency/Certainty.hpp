/*
 * Certainty.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_CERTAINTY_HPP_FILE
#define INCLUDE_PERSISTENCY_CERTAINTY_HPP_FILE

/* public header */

#include <boost/operators.hpp>
#include <cassert>

#include "Persistency/Exception.hpp"


namespace Persistency
{

/** \brief exception on invalid certanity level.
 */
struct ExceptionInvalidCertainty: public Exception
{
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   */
  ExceptionInvalidCertainty(const Location &where):
    Exception(where, "certainty out of bound requested")
  {
  }
}; // struct ExceptionInvalidCertainty


/** \brief certanity level representation.
 */
class Certainty: public boost::equality_comparable<Certainty>
{
public:
  /** \brief create certanity representation [0;1].
   *  \param c certanity value.
   *  \note c-tor is not explicit to make usage simpler.
   */
  Certainty(const double c=1):
    c_(c)
  {
    if(c_<0 || 1<c_)
      throw ExceptionInvalidCertainty(SYSTEM_SAVE_LOCATION);
  }
  /** \brief gets certanity level.
   *  \return certanity saved in class.
   */
  double get(void) const
  {
    assert(0<=c_);
    assert(c_<=1);
    return c_;
  }
  /** \brief check if classes are equal.
   *  \param other element to compare with.
   *  \return true if elements are equal, false otherwise.
   */
  bool operator==(const Certainty &other) const
  {
    return get()==other.get();
  }

private:
  double c_;
}; // class Certainty

} // namespace Presistency

#endif
