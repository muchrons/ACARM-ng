/*
 * EntryReader.cpp
 *
 */
#include <sstream>
#include <cassert>

#include "Persistency/IO/Postgres/detail/EntryReader.hpp"
#include "Persistency/IO/Postgres/TransactionAPI.hpp"

using namespace std;
using namespace pqxx;

using Persistency::IO::Transaction;

namespace Persistency
{
namespace IO
{
namespace Postgres
{
namespace detail
{

EntryReader::EntryReader(Transaction &t, DBHandler &dbh):
  dbh_(dbh),
  t_(t)
{
}

//TODO: work in progress
/*
Alert &EntryReader::readAlert(DataDaseID alertID)
{
  stringstream ss;
  ss << "SELECT * FROM alerts where id = " << alertID << ";";
  result r = t_.getAPI<TransactionAPI>().exec(ss);

  string name, description, detect_time, create_time;
  int id_severity;
  double caertanity;

  r[0]["name"].to(name);
  r[0]["description"].to(description);
  r[0]["create_time"].to(create_time);


}

Alert::SourceAnalyzers &EntryReader::getAnalyzers(DataBaseID alertID)
{
  stringstream ss;
  ss << "SELECT * FROM alert_analyzers WHERE id_alert = " << alertID <<";";
  result r = t_.getAPI<TransactionAPI>().exec(ss);
  Alert::SourceAnalyzers analyzers;

  for(unsigned int i=0; i<r.size(); ++i)
  {
    DataBaseID id;
    r[i]["id_analyzer"].to(id);

    stringstream sa;
    sa << "SELECT * FROM analyzers WHERE id = " << id << ";";
    result ra = t_.getAPI<TransactionAPI>().exec(sa);

    string name, version, os, ip;
    ra[0]["name"].to(name);
    ra[0]["version"].to(version);
    ra[0]["os"].to(os);
    ra[0]["ip"].to(ip);

    const Analyzer anlz(name,
                        Analyzer::Version(version),
                        Analyzer::OS(os),
                        Analyzer::IPv4::from_string(ip) );
    analyzers.push_back(anlz);
  }
  return analyzers;
}
*/
} // namespace detail
} // namespace Postgres
} // namespace IO
} // namespace Persistency
