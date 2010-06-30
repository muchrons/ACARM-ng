/*
 * CommonData.hpp
 *
 */
#ifndef INCLUDE_INPUT_COMMONDATA_HPP_FILE
#define INCLUDE_INPUT_COMMONDATA_HPP_FILE

/* public header */

#include <boost/noncopyable.hpp>

#include "Commons/SharedPtrNotNULL.hpp"
#include "Persistency/IO/DynamicConfig.hpp"
#include "Input/detail/IDMap.hpp"

namespace Input
{

/** \brief common data shared between inputs.
 */
class CommonData: private boost::noncopyable
{
public:
  explicit CommonData(Persistency::IO::DynamicConfig &dynConf);

  detail::IDMap &getIDMap(void)
  {
    return idMap_;
  }

private:
  detail::IDMap idMap_;
}; // class CommonData


/** \brief non-NULL pointer to common inputs' data. */
typedef Commons::SharedPtrNotNULL<CommonData> CommonDataPtrNN;

} // namespace Input

#endif
