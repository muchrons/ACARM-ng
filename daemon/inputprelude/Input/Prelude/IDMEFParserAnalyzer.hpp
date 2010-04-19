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
  /**
   * @brief c-tor
   * \param ptr idmef_analyzer_t structure to parse
   */
  explicit IDMEFParserAnalyzer(idmef_analyzer_t *ptr);

  /**
   * @brief gets name of an analyzer
   */
  const Persistency::Analyzer::Name& getName() const;

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
  idmef_analyzer_t * isEmpty(idmef_analyzer_t *ptr) const;
  Persistency::Analyzer::Name parseName(idmef_analyzer_t *ptr) const;
  std::auto_ptr<Persistency::Analyzer::Version> parseVersion(idmef_analyzer_t *ptr) const;
  std::auto_ptr<Persistency::Analyzer::OS> parseOs(idmef_analyzer_t *ptr) const;
  std::auto_ptr<Persistency::Analyzer::IP> parseIP(idmef_analyzer_t *ptr) const;

  Persistency::Analyzer::Name                   name_;
  std::auto_ptr<Persistency::Analyzer::Version> version_;
  std::auto_ptr<Persistency::Analyzer::OS>      os_;
  std::auto_ptr<Persistency::Analyzer::IP>      ip_;
}; // class IDMEFParserAnalyzer

} // namespace Prelude
} // namespace Input

#endif
