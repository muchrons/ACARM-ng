/*
 * SomeInputBuilder.cpp
 *
 */
#include <tut/tut.hpp>
#include <string>
#include <unistd.h>

#include "ConfigIO/InputConfig.hpp"
#include "Commons/Factory/RegistratorHelper.hpp"
#include "Input/Factory.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace std;
using namespace Input;
using namespace Core::Types::Proc;

namespace
{

struct SomeInput: public Reader
{
  SomeInput(void):
    Reader( TypeName("someinput"), InstanceName("someinputname") ),
    firstRun_(true)
  {
  }

  virtual DataPtr read(BackendFacade &, unsigned int)
  {
    if(!firstRun_)
      usleep(50*1000);  // limit created objects count a little...
    firstRun_=false;
    return TestHelpers::Persistency::makeNewAlert().shared_ptr();
  }

  bool firstRun_;
}; // struct SomeInput

class SomeInputBuilder: public Factory::TFactoryBuilderBase
{
public:
  /** \brief creates builder of a readers for Input::Prelude.
   */
  SomeInputBuilder(void):
    name_("someinput")
  {
  }

private:
  virtual FactoryPtr buildImpl(const Options &/*options*/) const
  {
    return Factory::FactoryPtr(new SomeInput);
  }
  virtual const FactoryTypeName &getTypeNameImpl(void) const
  {
    return name_;
  }

  const std::string name_;
}; // class ReaderBuilder

// this code registers builder to readers factory, making it available for
// further usage, by generic interface.
const Commons::Factory::RegistratorHelper<Factory, SomeInputBuilder> g_rh;

} // unnamed namespace
