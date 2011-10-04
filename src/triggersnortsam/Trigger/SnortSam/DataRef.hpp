/*
 * DataRef.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_SNORTSAM_DATAREF_HPP_FILE
#define INCLUDE_TRIGGER_SNORTSAM_DATAREF_HPP_FILE

#include <cstdlib>
#include <inttypes.h>

#include "Trigger/SnortSam/Exception.hpp"

namespace Trigger
{
namespace SnortSam
{

/** \brief gives read-only access to the internal buffers.
 */
struct DataRef
{
  /** \brief create data reference w/o ownership.
   *  \param data data to be held.
   *  \param size length of the data bufer.
   */
  DataRef(const uint8_t *data, const size_t size):
    data_(data),
    size_(size)
  {
    if(data==NULL)
      throw Exception(SYSTEM_SAVE_LOCATION, "data pointer cannot be NULL");
  }

  const uint8_t *data(void) const
  {
    assert(data_!=NULL);
    return data_;
  }

  uint8_t operator[](const size_t pos) const
  {
    assert(pos<size() && "index out of bound");
    return data()[pos];
  }

  size_t size(void) const
  {
    return size_;
  }

private:
  const uint8_t *data_;
  size_t         size_;
}; // struct DataRef

} // namespace SnortSam
} // namespace Trigger

#endif
