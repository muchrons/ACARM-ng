/*
 * IDMEFParserAnalyzer.hpp
 *
 */
#ifndef INCLUDE_INPUT_PRELUDE_IDMEFPARSERANALYZER_HPP_FILE
#define INCLUDE_INPUT_PRELUDE_IDMEFPARSERANALYZER_HPP_FILE

#include <string>
#include <prelude.h>

#include "Persistency/Alert.hpp"
#include "Persistency/Timestamp.hpp"
#include "Persistency/Analyzer.hpp"
#include "Persistency/ReferenceURL.hpp"
#include "Logger/Node.hpp"
#include "Input/Prelude/Exception.hpp"

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
  /** \brief exception thrown when ID of analyzer is missing.
   */
  struct ExceptionMissingID: public Exception
  {
    /** \brief create execption with given message.
     *  \param where place where exception has been thrown.
     *  \param msg   message to represent.
     */
    explicit ExceptionMissingID(const Location &where):
      Exception(where, "analyzer ID is not present in data")
    {
    }

  }; // struct ExceptionMissingID

  /**
   * @brief c-tor
   * \param ptr idmef_analyzer_t structure to parse
   */
  explicit IDMEFParserAnalyzer(idmef_analyzer_t *ptr);

  /**
   * @brief gets ID assigned to this analyzer by prelude.
   */
  const std::string &getPreludeID() const;
  /**
   * @brief gets name of an analyzer
   */
  const Persistency::Analyzer::Name& getName() const;

  /**
   * @brief gets version of an analyzer
   */
  const Persistency::Analyzer::Version &getVersion() const;

  /**
   * @brief gets OS version this analyzer is running on
   */
  const Persistency::Analyzer::OperatingSystem &getOS() const;

  /**
   * @brief gets IP address of an analyzer
   */
  const Persistency::Analyzer::IP* getIP() const;

private:
  idmef_analyzer_t * getNonNull(idmef_analyzer_t *ptr) const;

  Persistency::Analyzer::Name parseName(idmef_analyzer_t *ptr) const;
  std::string parsePreludeID(idmef_analyzer_t *ptr) const;
  Persistency::Analyzer::Version parseVersion(idmef_analyzer_t *ptr) const;
  Persistency::Analyzer::OperatingSystem parseOS(idmef_analyzer_t *ptr) const;
  std::auto_ptr<Persistency::Analyzer::IP> parseIP(idmef_analyzer_t *ptr) const;

  const Logger::Node                       log_;
  std::string                              preludeID_;
  Persistency::Analyzer::Name              name_;
  Persistency::Analyzer::Version           version_;
  Persistency::Analyzer::OperatingSystem   os_;
  // TODO: auto_ptr is over kill here - consider using Base::NullValue<> for this
  std::auto_ptr<Persistency::Analyzer::IP> ip_;
}; // class IDMEFParserAnalyzer

} // namespace Prelude
} // namespace Input

#endif
