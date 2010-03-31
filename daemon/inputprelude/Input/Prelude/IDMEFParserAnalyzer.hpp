/*
 * IDMEFParserAnalyzer.hpp
 *
 */
#ifndef INCLUDE_INPUT_PRELUDE_IDMEFPARSERANALYZER_HPP_FILE
#define INCLUDE_INPUT_PRELUDE_IDMEFPARSERANALYZER_HPP_FILE

// TODO: "prelude.h"
#include <prelude.h>

#include "Persistency/Alert.hpp"
#include "Persistency/Timestamp.hpp"
#include "Persistency/Analyzer.hpp"
#include "Persistency/ReferenceURL.hpp"

namespace Input
{
namespace Prelude
{
/**
 * @brief Extracts Analyzer related data from Prelude IDMEF structures
 */
class IDMEFParserAnalyzer
{
public:
  // TODO: c-tor should be explicit
  /**
   * @brief c-tor
   * \param ptr idmef_analyzer_t structure to parse
   */
  IDMEFParserAnalyzer(idmef_analyzer_t *ptr);

  // TODO: const reference should be returned
  /**
   * @brief gets name of an analyzer
   */
  Persistency::Analyzer::Name getName() const;

  /**
   * @brief gets version of an analyzer
   */
  const Persistency::Analyzer::Version* getVersion() const;

  /**
   * @brief gets OS version this analyzer is running on
   */
  const Persistency::Analyzer::OS* getOS() const;

  /**
   * @brief gets IP address of an analyzer
   */
  const Persistency::Analyzer::IP* getIP() const;

private:
  // TODO: stict to names-in-one-column convention
  idmef_analyzer_t *ptr_;

  Persistency::Analyzer::Name name_;
  // TODO: NullValue<> should be used here - i'll port it from Persistency to
  //       Base component and let you know when done (ticket #258).
  boost::shared_ptr<Persistency::Analyzer::Version> version_;
  boost::shared_ptr<Persistency::Analyzer::OS> os_;
  boost::shared_ptr<Persistency::Analyzer::IP> ip_;
}; // class IDMEFParserAnalyzer

} // namespace Prelude
} // namespace Input

#endif
