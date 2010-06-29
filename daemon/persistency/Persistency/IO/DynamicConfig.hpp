/*
 * DynamicConfig.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_DYNAMICCONFIG_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_DYNAMICCONFIG_HPP_FILE

/* public header */

#include <string>
#include <memory>
#include <boost/noncopyable.hpp>

#include "Base/NullValue.hpp"
#include "Persistency/IO/Transaction.hpp"
#include "Persistency/IO/Exception.hpp"

namespace Persistency
{
namespace IO
{

/** \brief operations on dynamic configuration's of given element.
 */
class DynamicConfig: private boost::noncopyable
{
public:
  typedef Base::NullValue<std::string> StringNULL;
  /** \brief create meta-alert persistency proxy.
   *  \param owner owner's name of a configuration to work on (NULL means common config).
   *  \param t  active transaction.
   */
  DynamicConfig(const char *owner, Transaction &t);
  /** \brief virtual d-tor for polymorphic base class.
   */
  virtual ~DynamicConfig(void);

  /** \brief save given cofiguration key/value pair.
   *  \param key   name of the parameter.
   *  \param value parameter's value.
   *  \note if given parameter already exists, it will be overwritten.
   */
  void write(const std::string &key, const std::string &value);
  /** \brief read given configuration paramter.
   *  \param key name of the paramter to read.
   *  \return paramter's value, if it does exist, NULL otherwise.
   */
  StringNULL read(const std::string &key);
  /** \brief read given read-only configuration paramter.
   *  \param key name of the paramter to read.
   *  \return paramter's value.
   */
  std::string readConst(const std::string &key);

protected:
  /** \brief gets owner's name.
   *  \return owner's name (can be NULL).
   */
  const char *getOwner(void) const;

private:
  virtual void writeImpl(Transaction &t, const std::string &key, const std::string &value) = 0;
  virtual StringNULL readImpl(Transaction &t, const std::string &key) = 0;
  virtual std::string readConstImpl(Transaction &t, const std::string &key) = 0;

  StringNULL   owner_;
  Transaction &t_;
}; // class DynamicConfig


/** \brief auto pointer to IO::DynamicConfig class. */
typedef std::auto_ptr<DynamicConfig> DynamicConfigAutoPtr;

} // namespace IO
} // namespace Persistency

#endif
