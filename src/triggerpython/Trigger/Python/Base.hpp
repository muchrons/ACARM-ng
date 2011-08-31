/*
 * Base.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_BASE_HPP_FILE
#define INCLUDE_TRIGGER_BASE_HPP_FILE

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

#include "Logger/Logger.hpp"
#include "Commons/SharedPtrNotNULL.hpp"
#include "PythonAPI/Persistency/MetaAlert.hpp"
#include "Trigger/Simple/Strategy.hpp"
#include "Trigger/Python/ExceptionNoImplementation.hpp"

namespace Trigger
{
namespace Python
{

/** \brief base interface for strategy to operate on.
 */
class Base: private boost::noncopyable
{
public:
  /** \brief creates instance.
   */
  Base(void);
  /** \brief ensure secure, polymorphic destruction.
   */
  virtual ~Base(void);
  /** \brief forward call to the implementaiton.
   *  \param n node to process (trigger).
   */
  void trigger(const Simple::Strategy::ConstNode &n);

protected:
  const Logger::Node log_;  ///< logger to use

private:
  /** \brief interface to be implemented by derived class.
   *  \param ma proxy object to access node.
   */
  virtual void triggerImpl(PythonAPI::Persistency::MetaAlert ma) = 0;
}; // class Base


typedef boost::shared_ptr<Base>         BasePtr;
typedef Commons::SharedPtrNotNULL<Base> BasePtrNN;

} // namespace Python
} // namespace Trigger

#endif
