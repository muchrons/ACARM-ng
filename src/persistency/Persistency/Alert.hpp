/*
 * Alert.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_ALERT_HPP_FILE
#define INCLUDE_PERSISTENCY_ALERT_HPP_FILE

/* public header */

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

// TODO: implement
// TODO: test
// TODO: comment

namespace Persistency
{

/** \brief represents alert report got from input.
 */
class Alert: private boost::noncopyable
{
public:
  /** \brief starts virtual d-tors from the begining of the hierarchy.
   */
  virtual ~Alert(void)
  {
  }
}; // class Alert


/** \brief smart pointer to Alert type.
 */
typedef boost::shared_ptr<Alert> AlertPtr;

} // namespace Persistency

#endif
