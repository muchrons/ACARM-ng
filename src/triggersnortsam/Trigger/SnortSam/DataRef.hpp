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
   *  \note this class does NOT keeps ownership of data pointed to - use it with care.
   */
  DataRef(const uint8_t *data, const size_t size):
    data_(data),
    size_(size)
  {
    if(data==NULL)
      throw Exception(SYSTEM_SAVE_LOCATION, "data pointer cannot be NULL");
  }
  /** \brief gives read-only access to data pointer.
   *  \return non-NULL pointer to data.
   */
  const uint8_t *data(void) const
  {
    assert(data_!=NULL);
    return data_;
  }
  /** \brief array-like access to data bytes.
   *  \param pos byte to read.
   *  \return requested byte.
   *  \note class does NOT check ranges - use with care.
   */
  uint8_t operator[](const size_t pos) const
  {
    assert(pos<size() && "index out of bound");
    return data()[pos];
  }
  /** \brief gets size of the data array.
   *  \return number of bytes in data set. may be zero.
   */
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
