/*
 * CheckMode.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_CHECKMODE_HPP_FILE
#define INCLUDE_PREPROCESSOR_CHECKMODE_HPP_FILE

#include <boost/noncopyable.hpp>

#include "Persistency/Alert.hpp"


// TODO: comment

namespace Preprocessor
{

class CheckMode: private boost::noncopyable
{
public:
  virtual ~CheckMode(void);
  virtual bool check(const Persistency::Alert &alert) const = 0;
}; // class CheckMode

} // namespace Preprocessor

#endif
