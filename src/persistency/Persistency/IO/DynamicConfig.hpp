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
#include "Commons/LimitedString.hpp"
#include "Commons/LimitedNULLString.hpp"
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
  /** \brief owner type. */
  typedef Commons::LimitedNULLString<64>  Owner;
  /** \brief string representing key. */
  typedef Commons::LimitedString<64>      Key;
  /** \brief string representing value. */
  typedef Commons::LimitedString<256>     Value;
  /** \brief string to be read - can be NULL. */
  typedef Base::NullValue<Value>          ValueNULL;

  /** \brief base class for iteration over paramters.
   */
  struct IterationCallback: private boost::noncopyable
  {
    /** \brief ensures polymorphic destruciton.
     */
    virtual ~IterationCallback(void);

    /** \brief user-defined callback method.
     *  \param key   name of the parameter.
     *  \param value parameter's value.
     *  \return true, if iteration should proceed, false otherwise.
     */
    virtual bool process(const Key &key, const Value &value) = 0;
  }; // struct IterationCallback

  /** \brief exception thrown when given paramter does not exist.
   */
  struct ExceptionNoSuchParameter: public Exception
  {
    /** \brief create error message.
     *  \param where place where exception has been risen.
     *  \param owner parameter's owner.
     *  \param key   parameter name that is not set.
     */
    ExceptionNoSuchParameter(const Location &where, const Owner &owner, const Key &key):
      Exception(where, cc("no such read-only parameter: '", key.get(),
                          "' for user '", owner.get(), "'") )
    {
    }
  }; // struct ExceptionNoSuchParamter

  /** \brief create meta-alert persistency proxy.
   *  \param owner owner's name of a configuration to work on (NULL means common config).
   *               each owner has separate 'namespace', i.e. the same keys may repeat for
   *               different owners and they won't interfere.
   *  \param t     active transaction.
   */
  DynamicConfig(const Owner &owner, Transaction &t);
  /** \brief virtual d-tor for polymorphic base class.
   */
  virtual ~DynamicConfig(void);

  /** \brief save given cofiguration key/value pair.
   *  \param key   name of the parameter.
   *  \param value parameter's value.
   *  \note if given parameter already exists, it will be overwritten.
   */
  void write(const Key &key, const Value &value);
  /** \brief read given configuration paramter.
   *  \param key name of the paramter to read.
   *  \return paramter's value, if it does exist, NULL otherwise.
   */
  ValueNULL read(const Key &key);
  /** \brief read given read-only configuration paramter.
   *  \param key name of the paramter to read.
   *  \return paramter's value.
   *  \note const-paramters are common for all owners (since noone can write them anyway).
   */
  Value readConst(const Key &key);
  /** \brief remove given parameter from configuration.
   *  \param key name of the parameter.
   *  \note if given parameter does not exist, call does nothing.
   */
  void remove(const Key &key);
  /** \brief iterate over all paramters.
   *  \param cb callback called for each element.
   *  \note order in which parameters will be read is undefined.
   */
  void iterate(IterationCallback &cb);

protected:
  /** \brief gets owner's name.
   *  \return owner's name (can be NULL).
   */
  const Owner &getOwner(void) const;

private:
  virtual void writeImpl(Transaction &t, const Key &key, const Value &value) = 0;
  virtual ValueNULL readImpl(Transaction &t, const Key &key) = 0;
  virtual Value readConstImpl(Transaction &t, const Key &key) = 0;
  virtual void removeImpl(Transaction &t, const Key &key) = 0;
  virtual void iterateImpl(Transaction &t, IterationCallback &cb) = 0;

  const Owner  owner_;
  Transaction &t_;
}; // class DynamicConfig


/** \brief auto pointer to IO::DynamicConfig class. */
typedef std::auto_ptr<DynamicConfig> DynamicConfigAutoPtr;

} // namespace IO
} // namespace Persistency

#endif
