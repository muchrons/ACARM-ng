/*
 * Alert.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>

#include "Persistency/Alert.hpp"
#include "Persistency/TestHelpers.t.hpp"

using namespace std;
using namespace Persistency;

namespace
{

struct TestClass
{
  TestClass(void):
    name_("some name"),
    analyzer_( new Analyzer("analyzer name", HostPtr() ) ),
    detected_(),
    created_(),
    severity_(SeverityLevel::INFO),
    certanity_(42),
    description_("alert's description"),
    sourceHosts_( generateReportedHosts(2) ),
    targetHosts_( generateReportedHosts(5) )
  {
  }

  Alert::ReportedHosts generateReportedHosts(unsigned int size) const
  {
    Alert::ReportedHosts out;
    for(unsigned int i=0; i<size; ++i)
      out.push_back( makeNewHost() );
    return out;
  }

  const Alert::Name          name_;
  const AnalyzerPtr          analyzer_;
  const Timestamp            detected_;
  const Timestamp            created_;
  const Severity             severity_;
  const Certanity            certanity_;
  const string               description_;
  const Alert::ReportedHosts sourceHosts_;
  const Alert::ReportedHosts targetHosts_;
};

typedef TestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/Alert");
} // unnamed namespace


namespace tut
{

// test c-tor with all paramters set
template<>
template<>
void testObj::test<1>(void)
{
  const Alert a(name_, analyzer_, &detected_, created_, severity_, certanity_,
                &description_, sourceHosts_, targetHosts_);
  // check getters
  ensure_equals("invalid name", a.getName().get(), string( name_.get() ) );
  ensure("invalid analyzer", &a.getAnalyzer()==analyzer_.get() );
  ensure("NULL detect time", a.getDetectionTime()!=NULL);
  ensure_equals("invalid detect time", *a.getDetectionTime(), detected_);
  ensure_equals("invalid created time", a.getCreationTime(), created_);
  ensure_equals("invalid severity",
                a.getSeverity().getLevel().toInt(), severity_.getLevel().toInt() );
  ensure_equals("invalid certanity", a.getCertanity().get(), certanity_.get() );
  ensure("NULL description", a.getDescription()!=NULL);
  ensure_equals("invalid description", *a.getDescription(), description_);
  ensure_equals("invalid source hosts",
                a.getReportedSourceHosts().size(), sourceHosts_.size() );
  ensure_equals("invalid destination hosts",
                a.getReportedTargetHosts().size(), targetHosts_.size() );
}

// test c-tor with NULL detection time
template<>
template<>
void testObj::test<2>(void)
{
  const Alert a(name_, analyzer_, NULL, created_, severity_, certanity_,
                &description_, sourceHosts_, targetHosts_);
  ensure("detection time is not NULL", a.getDetectionTime()==NULL);
}

// test c-tor with NULL description
template<>
template<>
void testObj::test<3>(void)
{
  const Alert a(name_, analyzer_, &detected_, created_, severity_, certanity_,
                NULL, sourceHosts_, targetHosts_);
  ensure("description is not NULL", a.getDescription()==NULL);
}

} // namespace tut
