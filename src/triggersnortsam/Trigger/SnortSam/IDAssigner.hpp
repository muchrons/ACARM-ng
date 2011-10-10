/*
 * IDAssigner.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_SNORTSAM_IDASSIGNER_HPP_FILE
#define INCLUDE_TRIGGER_SNORTSAM_IDASSIGNER_HPP_FILE

#include <boost/noncopyable.hpp>

#include "Base/Threads/Mutex.hpp"
#include "Logger/Node.hpp"
#include "Commons/SharedPtrNotNULL.hpp"
#include "Trigger/BackendFacade.hpp"

namespace Trigger
{
namespace SnortSam
{

/** \brief class responsible for assigning IDs for new events to be sent.
 *
 *  it is not specified where will this class start counting from.
 *
 *  \warning only one instance of this class may exist at a time. this prevents
 *           errors related to assigning multiple IDs to different events.
 */
class IDAssigner: private boost::noncopyable
{
public:
  IDAssigner(void);

  unsigned int assign(BackendFacade &bf);

private:
  void read(BackendFacade &bf);
  void write(BackendFacade &bf);

  Base::Threads::Mutex                   mutex_;
  Logger::Node                           log_;
  Persistency::IO::DynamicConfig::Owner  owner_;
  const char                            *paramName_;
  unsigned int                           next_;
};

/** \brief non-NULL pointer to IDAssigner class. */
typedef Commons::SharedPtrNotNULL<IDAssigner> IDAssignerPtrNN;

} // namespace SnortSam
} // namespace Trigger

#endif
