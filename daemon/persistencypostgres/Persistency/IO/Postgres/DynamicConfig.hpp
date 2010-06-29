/*
 * DynamicConfig.t.cpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_DYNAMICCONFIG_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_DYNAMICCONFIG_HPP_FILE

#include "Persistency/IO/DynamicConfig.hpp"

namespace Persistency
{
namespace IO
{
namespace Postgres
{

/** \brief postgres implementaiton of DynamicConfig
 */
struct DynamicConfig: public Persistency::IO::DynamicConfig
{
public:
  /** \brief create instance for a given owner.
   *  \param owner owner's name.
   *  \param t     transaction to use.
   */
  DynamicConfig(const char *owner, Persistency::IO::Transaction &t);

private:
  virtual void writeImpl(Persistency::IO::Transaction &t, const std::string &key, const std::string &value);
  virtual StringNULL readImpl(Persistency::IO::Transaction &t, const std::string &key);
  virtual std::string readConstImpl(Persistency::IO::Transaction &t, const std::string &key);
}; // class DynamicConfig

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
