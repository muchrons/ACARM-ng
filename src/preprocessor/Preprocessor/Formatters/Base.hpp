/*
 * Base.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_FORMATTERS_BASE_HPP_FILE
#define INCLUDE_PREPROCESSOR_FORMATTERS_BASE_HPP_FILE

#include <string>
#include <vector>
#include <boost/noncopyable.hpp>

#include "Logger/Node.hpp"
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
  typedef std::vector<std::string> Arguments;
  typedef std::vector<BasePtrNN>   Formatters;

  /** \brief virtual d-tor to ensure proper destruction.
   */
  virtual ~Base(void);

  /** \brief check if given string matches predefined condition.
   *  \param str string to check.
   *  \return true if string matches condition, false otherwise.
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
