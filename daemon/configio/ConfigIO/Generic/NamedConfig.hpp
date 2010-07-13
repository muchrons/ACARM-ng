/*
 * NamedConfig.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_GENERIC_NAMEDCONFIG_HPP_FILE
#define INCLUDE_CONFIGIO_GENERIC_NAMEDCONFIG_HPP_FILE

/* public header */

#include <string>
#include <map>

#include "ConfigIO/Generic/Config.hpp"
#include "ConfigIO/ExceptionNoSuchParameter.hpp"


namespace ConfigIO
{
namespace Generic
{

/** \brief class representing configuration of persistency storage.
 *
 *  class is CRTP to prevent different class from being casted down
 *  to single base.
 */
template<typename CRTP>
class NamedConfig : public Config<CRTP>
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
  /** \brief gets persistency storage name.
   *  \return persitency name.
   */
  const TypeName &getName(void) const
  {
    return name_;
  }

protected:
  /** \brief creates persistency configuration.
   *  \param type    persistency type.
   *  \param type    persistency name.
   *  \param options options for persistency.
   */
  NamedConfig(const TypeName &type,
              const TypeName &name,
              const Options  &options):
    Config<CRTP>(type, options),
    name_(name)
  {
  }

  /** \brief ensure it's not possible to destroy this class via base.
   *
   *  it is done this way to avoid virtual d-tors.
   */
  ~NamedConfig(void)
  {
  }

private:
  TypeName name_;
}; // class ConfigNamed

} // namespace Generic
} // namespace ConfigIO

#endif
