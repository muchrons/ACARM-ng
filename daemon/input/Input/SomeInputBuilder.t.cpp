/*
 * SomeInputBuilder.t.cpp
 *
 */
#include <tut/tut.hpp>
#include <sstream>
#include <cassert>
#include <string>

#include "ConfigIO/InputConfig.hpp"
#include "Commons/Factory/RegistratorHelper.hpp"
#include "Input/Factory.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace std;
using namespace Input;

namespace
{

struct SomeInput: public Reader
{
  SomeInput(void):
    Reader("someinput")
  {
  }

  virtual DataPtr read(BackendFacade &, unsigned int)
  {
    return DataPtr();
  }
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
  virtual FactoryPtr buildImpl(const Options &options) const
  {
    const ConfigIO::InputConfig ic(name_, options);
    tut::ensure_equals("invalid option opt1", ic["opt1"], "the answer is: 42");

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
