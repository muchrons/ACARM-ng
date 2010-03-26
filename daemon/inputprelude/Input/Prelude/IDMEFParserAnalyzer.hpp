/*
 * IDMEFParserAnalyzer.hpp
 *
 */
#ifndef INCLUDE_INPUT_PRELUDE_IDMEFPARSERANALYZER_HPP_FILE
#define INCLUDE_INPUT_PRELUDE_IDMEFPARSERANALYZER_HPP_FILE

#include <libprelude/prelude.h>


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
  IDMEFParserAnalyzer(idmef_analyzer_t *ptr);  

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
  idmef_analyzer_t *ptr_;

  Persistency::Analyzer::Name name_;
  boost::shared_ptr<Persistency::Analyzer::Version> version_;
  boost::shared_ptr<Persistency::Analyzer::OS> os_;
  boost::shared_ptr<Persistency::Analyzer::IP> ip_;
};//class IDMEFParserAnalyzer

} // namespace Prelude
} // namespace Input

#endif
