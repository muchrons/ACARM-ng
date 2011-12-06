/*
 * FormatterConfig.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_PREPROCESSOR_FORMATTERCONFIG_HPP_FILE
#define INCLUDE_CONFIGIO_PREPROCESSOR_FORMATTERCONFIG_HPP_FILE

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
  typedef std::vector<FormatterConfigData> Arguments;

  enum Type
  {
    FUNCTION,
    ARGUMENT,
    VALUE
  };

  Type        type_;
  std::string str_;
  Arguments   args_;
}; // struct FormatterConfigData
} // namespace detail


/** \brief configuration of formatter.
 */
class FormatterConfig
{
public:
  class Wrapper
  {
  public:
    typedef detail::FormatterConfigData::Arguments Arguments;

    explicit Wrapper(const detail::FormatterConfigData &fcd):
      fcd_(&fcd)
    {
    }

    bool isFunction(void) const
    {
      assert(fcd_!=NULL);
      return fcd_->type_==detail::FormatterConfigData::FUNCTION;
    }
    bool isArgument(void) const
    {
      assert(fcd_!=NULL);
      return fcd_->type_==detail::FormatterConfigData::ARGUMENT;
    }
    bool isValue(void) const
    {
      assert(fcd_!=NULL);
      return fcd_->type_==detail::FormatterConfigData::VALUE;
    }

    const std::string &argument(void) const
    {
      assert( isArgument() && "cannot request getting arguments value on non-parameter" );
      assert(fcd_!=NULL);
      return fcd_->str_;
    }

    const std::string &name(void) const
    {
      assert( isFunction() && "cannot request name on non-function" );
      assert(fcd_!=NULL);
      return fcd_->str_;
    }

    size_t argCount(void) const
    {
      assert( isFunction() && "cannot request paramter count on non-function" );
      assert(fcd_!=NULL);
      return fcd_->args_.size();
    }

    Wrapper param(size_t pos) const
    {
      assert( isFunction() && "cannot request paramter on non-function" );
      assert(fcd_!=NULL);
      return Wrapper( fcd_->args_.at(pos) );
    }

  private:
    const detail::FormatterConfigData *fcd_;
  }; // class Wrapper


  explicit FormatterConfig(const detail::FormatterConfigData &fcd):
    fcd_(fcd)
  {
  }

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
