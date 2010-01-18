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
#include "ConfigIO/LoggerConfig.hpp"

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

  /** \brief gets persistency configuration.
   *  \return configration of persistency module.
   */
  const PersistencyConfig &persistencyConfig(void) const;
  /** \brief gets logger configuration.
   *  \return configration of logger module.
   */
  const LoggerConfig &loggerConfig(void) const;

private:
  friend class System::Singleton<SingletonImpl>;
  SingletonImpl(void);

  boost::scoped_ptr<Parser> p_;
}; // class SingletonImpl



typedef System::Singleton<SingletonImpl> Singleton;

} // namespace ConfigIO

#endif
