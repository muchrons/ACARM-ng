/*
 * IDMEFParserAnalyzer.hpp
 *
 */
#ifndef INCLUDE_INPUT_PRELUDE_IDMEFPARSERANALYZER_HPP_FILE
#define INCLUDE_INPUT_PRELUDE_IDMEFPARSERANALYZER_HPP_FILE

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
  /*
   * @brief c-tor
   * $param ptr 
   */
  IDMEFParserAnalyzer(idmef_analyzer_t *ptr);  
  Persistency::Analyzer::Name getName() const;
  Persistency::Analyzer::Version getVersion() const;
  Persistency::Analyzer::OS getOS() const;
  Persistency::Analyzer::IP getIP() const;

private:
  idmef_analyzer_t *ptr_;
};

} // namespace Prelude
} // namespace Input

#endif
