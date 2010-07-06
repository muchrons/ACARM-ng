/*
 * ObjectID.hpp
 *
 */
#ifndef INCLUDE_BASE_OBJECTID_HPP_FILE
#define INCLUDE_BASE_OBJECTID_HPP_FILE

/* public header */

#include <inttypes.h>
#include <boost/operators.hpp>

// TODO: tests

namespace Base
{

/** \brief object representing ID of a ginven object type.
 *  \param TOwner type that will be using specialization as its ID. this ensures
 *                IDs from different types will not mess together.
 */
template<typename TOwner>
class ObjectID: public boost::equivalent< ObjectID<TOwner> >,
                public boost::less_than_comparable< ObjectID<TOwner> >,
                public boost::equality_comparable< ObjectID<TOwner> >
{
public:
  /** \brief numeric representation of ID. */
  typedef uint64_t Numeric;

  /** \brief create instance with a given ID.
   *  \param id ID to be assigned.
   *  \note c-tor is implicit on purpose.
   */
  ObjectID(Numeric id):
    id_(id)
  {
  }
  /** \brief returns number representing this ID.
   *  \return ID number.
   */
  Numeric get(void) const
  {
    return id_;
  }

  /** \brief comparison operator (used for strict-weak-ordering.
   *  \param other object to compare with.
   *  \return true if *this<other, false otherwise.
   */
  bool operator<(const ObjectID<TOwner> &other) const
  {
    return get()<other.get();
  }

private:
  Numeric id_;
}; // class ObjectID

} // namespace Base

#endif
