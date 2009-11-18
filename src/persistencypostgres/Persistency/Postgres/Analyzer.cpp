/*
 * Analyzer.cpp
 *
 */

#include "Persistency/Postgres/Analyzer.hpp"


namespace Persistency
{
namespace Postgres
{

Analyzer::Analyzer(const AnalyzerAPI::Name &name,
                   HostPtr                  host):
  Persistency::Analyzer(name, host)
{
}

} // namespace Postgres
} // namespace Persistency
