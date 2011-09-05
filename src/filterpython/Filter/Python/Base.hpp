/*
 * Base.hpp
 *
 */
#ifndef INCLUDE_FILTER_PYTHON_BASE_HPP_FILE
#define INCLUDE_FILTER_PYTHON_BASE_HPP_FILE

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

#include "Logger/Logger.hpp"
#include "Commons/SharedPtrNotNULL.hpp"
#include "PythonAPI/Persistency/MetaAlert.hpp"
#include "Filter/Simple/Strategy.hpp"
#include "Filter/Python/ExceptionNoImplementation.hpp"

namespace Filter
{
namespace Python
{

/** \brief base interface for strategy to operate on.
 */
class Base: private boost::noncopyable
{
public:
  /** \brief ensure secure, polymorphic destruction.
   */
  virtual ~Base(void);
  /** \brief forward call to the implementaiton.
   *  \param n node to process (filter).
   */
  //void filter(const Persistency::ConstGraphNodePtrNN &n);

protected:
  /** \brief creates instance.
   */
  Base(void);

  const Logger::Node log_;  ///< logger to use

private:
  /** \brief interface to be implemented by derived class.
   *  \param ma proxy object to access node.
   */
  //virtual void filterImpl(PythonAPI::Persistency::MetaAlert ma) = 0;
}; // class Base


typedef boost::shared_ptr<Base>         BasePtr;
typedef Commons::SharedPtrNotNULL<Base> BasePtrNN;

} // namespace Python
} // namespace Filter

#endif
