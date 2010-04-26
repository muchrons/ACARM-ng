/*
 * Rule.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_PREPROCESSOR_RULE_HPP_FILE
#define INCLUDE_CONFIGIO_PREPROCESSOR_RULE_HPP_FILE

/* public header */

#include <string>

#include "System/Enum.hpp"

// TODO: comment this code.

namespace ConfigIO
{
namespace Preprocessor
{
namespace detail
{
/** \brief implementation helper.
 */
struct RuleType
{
  /** \brief implementaiton enum. only to be used via Rule::Type. */
  typedef enum
  {
    TRUE,
    FALSE,
    RULE
  } Type;
};

/** \brief implementation helper.
 */
struct RuleMode
{
  /** \brief implementaiton enum. only to be used via Rule::Mode. */
  typedef enum
  {
    EQUALS,
    CONTAINS
  } Type;
};
} // namespace detail


/** \brief class representing single-rule configuration.
 */
class Rule
{
public:
  /** \brief type of rule ot be used. */
  typedef System::Enum<detail::RuleType> Type;
  typedef std::string                    Path;
  typedef System::Enum<detail::RuleMode> Mode;
  typedef std::string                    Value;

  static Rule makeTrue(void)
  {
    return Rule(Type::TRUE, "", Mode::EQUALS, "");
  }
  static Rule makeFalse(void)
  {
    return Rule(Type::FALSE, "", Mode::EQUALS, "");
  }
  static Rule makeRule(const Path &path, Mode mode, const Value &value)
  {
    return Rule(Type::RULE, path, mode, value);
  }

  Type getType(void) const
  {
    return type_;
  }
  const Path &getPath(void) const
  {
    return path_;
  }
  Mode getMode(void) const
  {
    return mode_;
  }
  const Value &getValue(void) const
  {
    return value_;
  }

private:
  Rule(Type type, const Path &path, Mode mode, const Value &value):
    type_(type),
    path_(path),
    mode_(mode),
    value_(value)
  {
  }

  Type  type_;
  Path  path_;
  Mode  mode_;
  Value value_;
}; // class Rule

} // namespace Preprocessor
} // namespace ConfigIO

#endif
