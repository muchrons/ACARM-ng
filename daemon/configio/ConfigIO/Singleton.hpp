/*
 * Singleton.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_SINGLETON_HPP_FILE
#define INCLUDE_CONFIGIO_SINGLETON_HPP_FILE

/* public header */

#include <boost/noncopyable.hpp>

#include "System/Singleton.hpp"
#include "ConfigIO/PersistencyConfig.hpp"
#include "ConfigIO/FiltersConfigCollection.hpp"
#include "ConfigIO/TriggersConfigCollection.hpp"
#include "ConfigIO/InputsConfigCollection.hpp"
#include "ConfigIO/LoggerConfig.hpp"
#include "ConfigIO/Preprocessor/Config.hpp"

namespace ConfigIO
{

// forward declaration
class Parser;

/** \brief configuration access point.
 */
class SingletonImpl: private boost::noncopyable
{
public:
  /** \brief deallocate object.
   */
  ~SingletonImpl(void);
  /** \brief re-reads configuration file.
   *  \param path path to configuration file (NULL means: use default).
   *
   *  configuration is read from file and current config is substituted
   *  with new one.
   *  exception is thrown when new config parsing fails.
   */
  void rereadConfig(const char *path=NULL);

  /** \brief gets persistency configuration.
   *  \return configration of persistency module.
   */
  const PersistencyConfig &persistencyConfig(void) const;
  /** \brief gets logger configuration.
   *  \return configration of logger module.
   */
  const LoggerConfig &loggerConfig(void) const;
  /** \brief gets filters' configuration, read from file.
   *  \return filters' configuration.
   */
  const FiltersConfigCollection &filtersConfig(void) const;
  /** \brief gets triggers' configuration, read from file.
   *  \return triggers' configuration.
   */
  const TriggersConfigCollection &triggersConfig(void) const;
  /** \brief gets inputs' configuration, read from file.
   *  \return inputs' configuration.
   */
  const InputsConfigCollection &inputsConfig(void) const;
  /** \brief gets preprocessor's configuration.
   *  \return preprocessor's configuration.
   */
  const Preprocessor::Config &preprocessorConfig(void) const;

private:
  friend class System::Singleton<SingletonImpl>;
  SingletonImpl(void);

  boost::scoped_ptr<Parser> p_;
}; // class SingletonImpl



typedef System::Singleton<SingletonImpl> Singleton;

} // namespace ConfigIO

#endif
