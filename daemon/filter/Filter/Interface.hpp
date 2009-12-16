/*
 * Interface.hpp
 *
 */
#ifndef INCLUDE_FILTER_INTERFACE_HPP_FILE
#define INCLUDE_FILTER_INTERFACE_HPP_FILE

/* public header */

#include <boost/noncopyable.hpp>

#include "Logger/Logger.hpp"
//#include "

// TODO: this code is far-from-draft

// TODO: to continue this thread-safe dequeue is needed.

namespace Filter
{

/** \brief interface (base) for all filters.
 */
class Interface: private boost::noncopyable
{
public:
  /** \brief processes given meta-alert.
   */
  void process(void);   // TODO

protected:
  /** \brief create instance.
   */
  Interface(void);

private:
  virtual void processImpl(void) = 0;   // TODO
}; // class Interface

} // namespace Filter

#endif
