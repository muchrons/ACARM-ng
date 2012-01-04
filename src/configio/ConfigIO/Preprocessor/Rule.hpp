/*
 * Rule.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_PREPROCESSOR_RULE_HPP_FILE
#define INCLUDE_CONFIGIO_PREPROCESSOR_RULE_HPP_FILE

/* public header */

#include <string>

#include "System/Enum.hpp"
#include "ConfigIO/Preprocessor/FormatterConfig.hpp"

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
    RULE_TRUE,
    RULE_FALSE,
    RULE
  }
  Type;
};

/** \brief implementation helper.
 */
struct RuleMode
{
  /** \brief implementaiton enum. only to be used via Rule::Mode. */
  typedef enum
  {
    LESSTHAN,       // string is lexagonically before the pattern
    GREATERTHAN,    // string is lexagonically after the pattern
    EQUALS,         // string is identical with the given pattern
    CONTAINS,       // string contains the given pattern
    REGEXP,         // string matches given regular expression
    REGEXPCI,       // string matches given regular expression (case insensitive)
    INSET           // string matches 
  }
  Type;
};
} // namespace detail


/** \brief class representing single-rule configuration.
 */
class Rule
{
public:
  /** \brief type of rule ot be used. */
  typedef System::Enum<detail::RuleType> Type;
  /** \brief type representing path to element. */
  typedef std::string                    Path;
  /** \brief type for rules mode specification. */
  typedef System::Enum<detail::RuleMode> Mode;
  /** \brief type representing value to check for. */
  typedef std::string                    Value;

  /** \brief named c-tor for 'true' condition.
   *  \return created object.
   */
  static Rule makeTrue(void);
  /** \brief named c-tor for 'false' condition.
   *  \return created object.
   */
  static Rule makeFalse(void);
  /** \brief named c-tor for dynamically computed condition.
   *  \param path      path to be checked.
   *  \param mode      operationg mode of the rule.
   *  \param value     value to compare with.
   *  \param formatter configuration of the formatter, to create rule with.
   *  \return created object.
   */
  static Rule makeRule(const Path &path, Mode mode, const Value &value, const FormatterConfig &formatter);

  /** \brief gets rule type.
   *  \return rule type.
   */
  Type getType(void) const
  {
    return type_;
  }
  /** \brief gets path element to check.
   *  \return path to element to be checked.
   */
  const Path &getPath(void) const
  {
    return path_;
  }
  /** \brief gets rule mode.
   *  \return rule mode.
   */
  Mode getMode(void) const
  {
    return mode_;
  }
  /** \brief gets value to compare against.
   *  \return value to be comapred.
   */
  const Value &getValue(void) const
  {
    return value_;
  }
  /** \brief gets configuration for the formatter.
   *  \return formatter's config.
   */
  const FormatterConfig &getFormatter(void) const
  {
    return formatter_;
  }

private:
  // use named c-tors to create these objects.
  Rule(Type type, const Path &path, Mode mode, const Value &value, const FormatterConfig &formatter);

  Type            type_;
  Path            path_;
  Mode            mode_;
  Value           value_;
  FormatterConfig formatter_;
}; // class Rule

} // namespace Preprocessor
} // namespace ConfigIO

#endif
