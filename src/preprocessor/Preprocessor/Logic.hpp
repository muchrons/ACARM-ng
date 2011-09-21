/*
 * Logic.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_LOGIC_HPP_FILE
#define INCLUDE_PREPROCESSOR_LOGIC_HPP_FILE

/* public header */

#include <boost/scoped_ptr.hpp>

#include "Persistency/Alert.hpp"

namespace Preprocessor
{

// forward declaration of implementation
class LogicImpl;

/** \brief main preprocessor's logic - interface.
 */
class Logic
{
public:
  /** \brief create object instance with configuration taken from ConfigIO.
   */
  Logic(void);
  /** \brief ensure proper destruction of object.
   */
  ~Logic(void);

  /** \brief checks if given alert is to be accepted according to defined rules.
   *  \param alert alert to be checked.
   *  \return true, if alert is to be accepted, false otherwise.
   */
  bool checkAccept(const Persistency::Alert &alert) const;

private:
  boost::scoped_ptr<LogicImpl> pimpl_;
}; // class Logic

} // namespace Preprocessor

#endif
