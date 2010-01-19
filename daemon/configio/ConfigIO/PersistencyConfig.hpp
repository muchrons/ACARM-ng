/*
 * PersistencyConfig.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_PERSISTENCYCONFIG_HPP_FILE
#define INCLUDE_CONFIGIO_PERSISTENCYCONFIG_HPP_FILE

/* public header */

#include <string>
#include <map>

#include "ConfigIO/Exception.hpp"

namespace ConfigIO
{

/** \brief class representing configuration of persistency storage.
 */
class PersistencyConfig
{
public:
  /** \brief name of the persistency type.
   */
  typedef std::string                 TypeName;
  /** \brief type representing parameter name.
   */
  typedef std::string                 Parameter;
  /** \brief type representing string value.
   */
  typedef std::string                 Value;
  /** \brief options map (paramter=>value).
   */
  typedef std::map<Parameter, Value>  Options;

  /** \brief creates persistency configuration.
   *  \param type    persistency type.
   *  \param options options for persistency.
   */
  PersistencyConfig(const TypeName &type,
                    const Options  &options);

  /** \brief gets persistency storage type.
   *  \return persitency type name.
   */
  const TypeName &getType(void) const
  {
    return type_;
  }

  /** \brief gets options for persistency.
   *  \return options map.
   */
  const Options &getOptions(void) const
  {
    return options_;
  }

private:
  TypeName type_;
  Options  options_;
}; // class PersistencyConfig

} // namespace ConfigIO

#endif
