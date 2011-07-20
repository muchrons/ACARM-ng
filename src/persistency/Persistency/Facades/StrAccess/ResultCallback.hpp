/*
 * ResultCallback.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_FACADES_STRACCESS_RESULTCALLBACK_HPP_FILE
#define INCLUDE_PERSISTENCY_FACADES_STRACCESS_RESULTCALLBACK_HPP_FILE

/* public header */

#include <string>
#include <vector>

namespace Persistency
{
namespace Facades
{
namespace StrAccess
{

/** \brief callbacks base class.
 *
 *  callbacks when given element is located by path.
 */
class ResultCallback
{
public:
  /** \brief ensure valid destruction.
   */
  virtual ~ResultCallback(void);

  /** \brief called when requrested for size of the collection.
   *  \param size colleciton size.
   */
  virtual void collectionSize(size_t size) = 0;
  /** \brief called when requested specific value.
   *  \param v value as string.
   */
  virtual void value(const std::string &v) = 0;
}; // class ResultCallback

} // namespace StrAccess
} // namespace Facades
} // namespace Persistency

#endif
