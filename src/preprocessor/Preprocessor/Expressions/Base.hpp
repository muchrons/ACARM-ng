/*
 * Base.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_EXPRESSIONS_BASE_HPP_FILE
#define INCLUDE_PREPROCESSOR_EXPRESSIONS_BASE_HPP_FILE

#include <boost/noncopyable.hpp>

#include "Commons/SharedPtrNotNULL.hpp"
#include "Persistency/GraphNode.hpp"
#include "Preprocessor/ExceptionInvalidExpression.hpp"

namespace Preprocessor
{
namespace Expressions
{

/** \brief base class for all expressions.
 */
class Base: private boost::noncopyable
{
public:
  /** \brief polymorphic d-tor.
   */
  virtual ~Base(void);

  /** \brief computes answer for given alert.
   *  \param node (meta-)alert to be chekced/computed.
   *  \return true if alert matches given criteria, false otherwise.
   */
  virtual bool compute(const Persistency::ConstGraphNodePtrNN &node) const = 0;
}; // class Base


typedef Commons::SharedPtrNotNULL<Base> BasePtrNN;

} // namespace Expressions
} // namespace Preprocessor

#endif
