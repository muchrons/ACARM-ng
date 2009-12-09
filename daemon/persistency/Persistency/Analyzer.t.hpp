/*
 * Analyzer.t.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_ANALYZER_T_HPP_FILE
#define INCLUDE_PERSISTENCY_ANALYZER_T_HPP_FILE

#include "Persistency/Analyzer.hpp"
#include "Persistency/Host.t.hpp"

namespace
{

struct AnalyzerTestImpl: public Persistency::Analyzer
{
private:
  typedef Persistency::Analyzer Analyzer;
public:

  AnalyzerTestImpl(const Analyzer::Name &name, Persistency::HostPtr host):
    Analyzer(name, host)
  {
  }

  static Persistency::AnalyzerPtr makeNew(const char *name="some analyzer")
  {
    return Persistency::AnalyzerPtr(
        new AnalyzerTestImpl(name, HostTestImpl::makeNew() ) );
  }
}; // struct AnalyzerTestImpl

} // unnamed namespace

#endif
