/*
 * IDAssignerValue.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IDASSIGNERVALUE_HPP_FILE
#define INCLUDE_PERSISTENCY_IDASSIGNERVALUE_HPP_FILE

/* public header */

#include "Persistency/MetaAlert.hpp"

namespace Persistency
{

/** \brief implementation that returns always pre-defined value.
 */
struct IDAssignerValue: public MetaAlert::IDAssigner
{
  /** \brief assigne value to be returned.
   */
  explicit IDAssignerValue(const MetaAlert::ID id):
    id_(id)
  {
  }
  /** \brief get assigned value.
   *  \return pre-defined value.
   */
  virtual MetaAlert::ID assign(IO::DynamicConfig &/*dc*/)
  {
    return id_;
  }

private:
  const MetaAlert::ID id_;
}; // struct IDAssignerValue

} // namespace Persistency

#endif
