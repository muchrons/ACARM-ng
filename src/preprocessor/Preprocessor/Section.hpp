/*
 * Section.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_SECTION_HPP_FILE
#define INCLUDE_PREPROCESSOR_SECTION_HPP_FILE

#include "System/Enum.hpp"
#include "ConfigIO/Preprocessor/Section.hpp"
#include "Persistency/GraphNode.hpp"
#include "Preprocessor/Expressions/Base.hpp"
#include "Preprocessor/Expressions/Set.hpp"

namespace Preprocessor
{
namespace detail
{
/** \brief helper definition of enums.
 */
struct SectionDecision
{
  /** \brief enum values to be used. */
  typedef enum
  {
    ACCEPT,     // accept alert
    REJECT,     // reject alert
    CONTINUE    // no match - check next section
  }
  Type;
}; // struct SectionDecision
} // unnamed namespace

/** \brief implementation of processing section.
 */
class Section
{
public:
  /** \brief type returned from decision mechanism.
   */
  typedef System::Enum<detail::SectionDecision> Decision;

  /** \brief create object instance from given configuration.
   *  \param cfg           configuration to be used.
   */
  explicit Section(const ConfigIO::Preprocessor::Section &cfg);

  /** \brief process given alert and return decision.
   *  \param node (meta-)alert to be checked.
   *  \return Decision::ACCEPT when this alert is to be acceopted,
   *          Decision::REJECT when this alert is to be rejected or
   *          Decision::CONTINUE if this section cannot decide
   *          faith of this alert.
   */
  Decision process(const Persistency::ConstGraphNodePtrNN &node) const;

private:
  Expressions::Set create(const ConfigIO::Preprocessor::Expression::Expressions &cfg) const;
  Expressions::BasePtrNN create(const ConfigIO::Preprocessor::Expression &cfg) const;
  Expressions::BasePtrNN create(const ConfigIO::Preprocessor::Rule &cfg) const;

  bool                   isAcceptType_;
  Expressions::BasePtrNN expr_;
}; // class LogicImpl

} // namespace Preprocessor

#endif
