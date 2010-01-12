/*
 * ConnectionBuilder.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_CONNECTIONBUILDER_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_CONNECTIONBUILDER_HPP_FILE

#include <string>

#include "Logger/Logger.hpp"
#include "Persistency/IO/BackendFactory.hpp"
#include "Persistency/IO/Postgres/IDCache.hpp"
#include "Persistency/IO/Postgres/Exception.hpp"

namespace Persistency
{
namespace IO
{
namespace Postgres
{

/** \brief builder for reader of prelude format.
 */
class ConnectionBuilder: public BackendFactory::TFactoryBuilderBase
{
public:
  /** \brief execption thrown when required options is not present.
   */
  struct ExceptionNoSuchOption: public Exception
  {
    /** \brief create message.
     *  \param where place where exception has been generated.
     *  \param name  name of the missing paramter.
     */
    ExceptionNoSuchOption(const char *where, const char *name):
      Exception(where, std::string("no such option: ") + ensureString(name) )
    {
    }
  }; // struct ExceptionNoSuchOption

  /** \brief creates builder of a object's facotry for Persistency::Postgres.
   */
  ConnectionBuilder(void);

private:
  virtual FactoryPtr buildImpl(const Options &options) const;
  virtual const FactoryTypeName &getTypeNameImpl(void) const;

  const std::string &getOption(const Options &options, const char *name) const;

  const std::string  name_;
  const Logger::Node log_;
  IDCachePtrNN       idCache_;
}; // class ConnectionBuilder

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
