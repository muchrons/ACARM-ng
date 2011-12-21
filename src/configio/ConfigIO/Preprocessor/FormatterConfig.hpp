/*
 * FormatterConfig.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_PREPROCESSOR_FORMATTERCONFIG_HPP_FILE
#define INCLUDE_CONFIGIO_PREPROCESSOR_FORMATTERCONFIG_HPP_FILE

/* public header */

#include <string>
#include <vector>
#include <cassert>

namespace ConfigIO
{
namespace Preprocessor
{
namespace detail
{
/** \brief configuration of formatter.
 *
 *  this is a raw data form, required by the parser. it is never to
 *  be used directly in user's code!
 */
struct FormatterConfigData
{
  /** \brief vector of arguments to represent. */
  typedef std::vector<FormatterConfigData> Arguments;

  /** \brief types of element, that can be represented. */
  enum Type
  {
    FUNCTION,
    ARGUMENT,
    VALUE
  };

  Type        type_;    ///< type of element.
  std::string str_;     ///< function name or argument's value.
  Arguments   args_;    ///< function's arguments list.
}; // struct FormatterConfigData
} // namespace detail


/** \brief configuration of formatter.
 */
class FormatterConfig
{
public:
  /** \brief helper wrapper that makes accessing internal data structure more intuitive.
   *  \warning objects of this class are tight with a FormatterConfig, that created it.
   *           all of created Wrapper objects must be destroyed before destroing creator.
   */
  class Wrapper
  {
  public:
    /** \brief vector of arguments to the function. */
    typedef detail::FormatterConfigData::Arguments Arguments;

    /** \brief create instance of the object, from internal data implementation.
     *  \param fcd data to represent.
     *  \note never call this on your own! use FormatterConfig::get() instead.
     */
    explicit Wrapper(const detail::FormatterConfigData &fcd):
      fcd_(&fcd)
    {
    }

    /** \brief returns true if object is a function, false otherwise.
     */
    bool isFunction(void) const
    {
      assert(fcd_!=NULL);
      return fcd_->type_==detail::FormatterConfigData::FUNCTION;
    }
    /** \brief return true if object is an argument, false otherwise.
     */
    bool isArgument(void) const
    {
      assert(fcd_!=NULL);
      return fcd_->type_==detail::FormatterConfigData::ARGUMENT;
    }
    /** \brief returns true if object is a value, false otherwise.
     */
    bool isValue(void) const
    {
      assert(fcd_!=NULL);
      return fcd_->type_==detail::FormatterConfigData::VALUE;
    }

    /** \brief returns argument's value.
     *  \return string with argument's value.
     *  \note can be called only on objects of an argument type.
     */
    const std::string &argument(void) const
    {
      assert( isArgument() && "cannot request getting arguments value on non-parameter" );
      assert(fcd_!=NULL);
      return fcd_->str_;
    }

    /** \brief returns function's name.
     *  \return name of the function.
     *  \note can be called only on objects of a function type.
     */
    const std::string &name(void) const
    {
      assert( isFunction() && "cannot request name on non-function" );
      assert(fcd_!=NULL);
      return fcd_->str_;
    }

    /** \brief returns function's arguments count.
     *  \return elements count in the arguments array.
     *  \note can be called only on objects of a function type.
     */
    size_t argCount(void) const
    {
      assert( isFunction() && "cannot request paramter count on non-function" );
      assert(fcd_!=NULL);
      return fcd_->args_.size();
    }

    /** \brief returns function's argument.
     *  \param pos item number.
     *  \return required paramter.
     *  \note can be called only on objects of a function type.
     */
    Wrapper param(size_t pos) const
    {
      assert( isFunction() && "cannot request paramter on non-function" );
      assert(fcd_!=NULL);
      return Wrapper( fcd_->args_.at(pos) );
    }

  private:
    const detail::FormatterConfigData *fcd_;
  }; // class Wrapper


  /** \brief creates config from accessor object from a given data.
   */
  explicit FormatterConfig(const detail::FormatterConfigData &fcd):
    fcd_(fcd)
  {
  }

  /** \brief creates wrapper object to be used by the user.
   *  \return wrapper object to main function.
   *  \note these objects must not live longer than this one.
   */
  Wrapper get(void) const
  {
    return Wrapper(fcd_);
  }

private:
  detail::FormatterConfigData fcd_;
}; // class FormatterConfig

} // namespace Preprocessor
} // namespace ConfigIO

#endif
