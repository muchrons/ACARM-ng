/*
 * Similarity.hpp
 *
 */
#ifndef INCLUDE_ALGO_DIFF_SIMILARITY_HPP_FILE
#define INCLUDE_ALGO_DIFF_SIMILARITY_HPP_FILE

/* public header */

#include <cmath>
#include <cassert>

#include "Algo/Exception.hpp"

namespace Algo
{
namespace Diff
{

/** \brief similarity representation (value from [0;1]).
 */
class Similarity
{
public:
  /** \brief exception thrown when similarity value is invalid.
   */
  struct ExceptionInvalidValue: public Exception
  {
    /** \brief create error report.
     *  \param where place where exception has been reasen.
     *  \param msg   error message.
     *  \param value value tat was invalid.
     */
    ExceptionInvalidValue(const Location &where, const char *msg, double value);
  }; // struct ExceptionInvalidValue


  /** \brief create object with rage-check.
   *  \param v value to represent.
   */
  Similarity(const double v):
    sum_(v),
    count_(1)
  {
    if( !std::isfinite(v) )
      throw ExceptionInvalidValue(SYSTEM_SAVE_LOCATION, "value is not finite (inf/nan)", v);
    if(v<0)
      throw ExceptionInvalidValue(SYSTEM_SAVE_LOCATION, "value too small", v);
    if(1<v)
      throw ExceptionInvalidValue(SYSTEM_SAVE_LOCATION, "value too big", v);
  }

  /** \brief named c-tor normalizing given value to [0;1] range.
   *  \param v value to normalize.
   *  \return valid object representing similarity.
   */
  static const Similarity normalize(const double v)
  {
    if(v<0)
      return Similarity(0);
    if(1<v)
      return Similarity(1);
    return Similarity(v);
  }

  /** \brief gets saved value.
   */
  double get(void) const
  {
    const double v=sum_/count_;
    assert(0<=v);
    assert(v<=1);
    return v;
  }

  /** \brief merges two similarities.
   *  \param s similarity to merge with.
   */
  void merge(const Similarity s)
  {
    sum_  +=s.sum_;
    count_+=s.count_;
    get();  // just in case
  }

private:
  double sum_;
  size_t count_;
}; // class Similarity

} // namespace Diff
} // namespace Algo

#endif
