/*
 * Config.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_GENERIC_CONFIG_HPP_FILE
#define INCLUDE_CONFIGIO_GENERIC_CONFIG_HPP_FILE

/* public header */

#include <string>
#include <map>

#include "XML/Node.hpp"
#include "Base/NullValue.hpp"
#include "ConfigIO/Preprocessor/Config.hpp"
#include "ConfigIO/ExceptionNoSuchParameter.hpp"


namespace ConfigIO
{
namespace Generic
{

/** \brief class representing configuration of given plugin.
 *
 *  class is CRTP to prevent different class from being casted down
 *  to single base.
 */
template<typename CRTP>
class Config
{
public:
  /** \brief name of the plugin-instance type used. */
  typedef std::string                           TypeName;
  /** \brief type representing parameter name. */
  typedef std::string                           Parameter;
  /** \brief type representing string value. */
  typedef std::string                           Value;
  /** \brief options map (paramter=>value). */
  typedef std::map<Parameter, Value>            Options;

  /** \brief gets type of the plugin-instance.
   *  \return type name.
   */
  const TypeName &getType(void) const
  {
    return type_;
  }

  /** \brief gets options for plugin.
   *  \return options map.
   */
  const Options &getOptions(void) const
  {
    return options_;
  }
  /** \brief access specific option or throw.
   *  \param param parameter's name.
   *  \return value corresponding to given parmater.
   */
  const Value &operator[](const Parameter &param) const
  {
    Options::const_iterator it=options_.find(param);
    if( it==options_.end() )
      throw ExceptionNoSuchParameter(SYSTEM_SAVE_LOCATION, param);
    return it->second;
  }
  /** \brief access specific option and return it's value or NULL if entry does not exist.
   *  \param param parameter's name.
   *  \return value corresponding to given parmater. may be NULL, if value is not set.
   */
  const char *get(const char *param) const
  {
    Options::const_iterator it=options_.find(param);
    if( it==options_.end() )
      return NULL;
    return it->second.c_str();
  }
  /** \brief get preprocessors configuration.
   *  \return poitner to preprocessors configuration. NULL if preprocessor is not set.
   */
  const Preprocessor::Config *getPreprocessorConfig(void) const
  {
    return preproc_.get();
  }

protected:
  /** \brief creates configuration instance.
   *  \param type    plugin-instance type.
   *  \param options options for plugin.
   */
  Config(const TypeName &type,
         const Options  &options):
    type_(type),
    options_(options)
  {
  }

  /** \brief creates configuration instance, with preprocessor.
   *  \param type    plugin-instance type.
   *  \param options options for plugin.
   */
  Config(const TypeName             &type,
         const Options              &options,
         const Preprocessor::Config &preproc):
    type_(type),
    options_(options),
    preproc_(preproc)
  {
  }

  /** \brief ensure it's not possible to destroy this class via base.
   *
   *  it is done this way to avoid virtual d-tors.
   */
  ~Config(void)
  {
  }

private:
  typedef Base::NullValue<Preprocessor::Config> PreprocCfg;

  TypeName   type_;
  Options    options_;
  PreprocCfg preproc_;
}; // class Config

} // namespace Generic
} // namespace ConfigIO

#endif
