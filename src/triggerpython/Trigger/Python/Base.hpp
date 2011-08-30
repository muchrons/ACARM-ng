/*
 * Base.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_BASE_HPP_FILE
#define INCLUDE_TRIGGER_BASE_HPP_FILE

#include <boost/noncopyable.hpp>

#include "Logger/Logger.hpp"
#include "PythonAPI/Persistency/MetaAlert.hpp"
#include "Trigger/Simple/Strategy.hpp"
#include "Trigger/Python/ExceptionNoImplementation.hpp"

namespace Trigger
{
namespace Python
{

/** \brief base interface
 */
class Base: private boost::noncopyable
{
public:
  Base(void);
  virtual ~Base(void);
  void trigger(const Simple::Strategy::ConstNode &n);

protected:
  const Logger::Node log_;

private:
  virtual void triggerImpl(PythonAPI::Persistency::MetaAlert ma) = 0;
}; // class Base

} // namespace Python
} // namespace Trigger

#endif
