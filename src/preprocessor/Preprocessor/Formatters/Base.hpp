/*
 * Base.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_FORMATTERS_BASE_HPP_FILE
#define INCLUDE_PREPROCESSOR_FORMATTERS_BASE_HPP_FILE

#include <string>
#include <vector>
#include <boost/noncopyable.hpp>

#include "Logger/Logger.hpp"
#include "Commons/SharedPtrNotNULL.hpp"


namespace Preprocessor
{
namespace Formatters
{

// forward declaration
class Base;

/** \brief smart pointer to the base class. */
typedef Commons::SharedPtrNotNULL<Base> BasePtrNN;


/** \brief base class for all formatters.
 */
class Base: private boost::noncopyable
{
public:
  /** \brief arguments vector, to pass to the implementaion. */
  typedef std::vector<std::string> Arguments;
  /** \brief vector of formatters taken as an arguments. */
  typedef std::vector<BasePtrNN>   Formatters;

  /** \brief virtual d-tor to ensure proper destruction.
   */
  virtual ~Base(void);

  /** \brief executes required formatter, to do its work.
   *  \return result of execution of the formatters.
   */
  std::string exec(void) const;

protected:
  /** \brief create object instance of a given action.
   *  \param name name of this action (used for logging).
   *  \param fmts formatters to take source data from.
   */
  Base(const std::string &name, const Formatters &fmts);

  const Logger::Node log_;      ///< logger to be used by derived classes

private:
  virtual std::string execImpl(const Arguments &args) const = 0;

  const Formatters fmts_;
}; // class Base

} // namespace Formatters
} // namespace Preprocessor

#endif
