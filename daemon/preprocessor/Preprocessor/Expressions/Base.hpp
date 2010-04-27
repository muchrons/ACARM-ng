/*
 * Base.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_EXPRESSIONS_BASE_HPP_FILE
#define INCLUDE_PREPROCESSOR_EXPRESSIONS_BASE_HPP_FILE

#include <boost/noncopyable.hpp>

#include "Commons/SharedPtrNotNULL.hpp"
#include "Persistency/Alert.hpp"
#include "Preprocessor/Path.hpp"
#include "Preprocessor/ExceptionInvalidExpression.hpp"

// TODO: comment this code

namespace Preprocessor
{
namespace Expressions
{

class Base: private boost::noncopyable
{
public:
  virtual ~Base(void);

  virtual bool compute(const Path &path, const Persistency::Alert &alert) const = 0;
}; // class Base


typedef Commons::SharedPtrNotNULL<Base> BasePtrNN;

} // namespace Expressions
} // namespace Preprocessor

#endif
