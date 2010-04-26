/*
 * GlobalLibPreludeInit.Cpp
 *
 */
#include <cassert>

#include "System/Threads/SafeInitLocking.hpp"
#include "Input/Prelude/GlobalLibPreludeInit.hpp"

namespace Input
{
namespace Prelude
{

namespace
{

// these are language-guaranteed to be initialized in compile-time
int  g_counter       =0;
bool g_wasInitialized=false;
bool g_end           =false;

// mutex protecting operations on global types
SYSTEM_MAKE_STATIC_SAFEINIT_MUTEX(g_mutex);

// helper object to perofrm release when program ends (after main()).
struct Releaser
{
  ~Releaser(void)
  {
    System::Threads::SafeInitLock lock(g_mutex);
    // this condition means that prelude is still being used somewhere...
    if(g_counter>0)
    {
      assert(g_wasInitialized==true);
      g_end=true;   // force GlobalLibPreludeInit to perform release
      return;
    }
    // ok - release!
    uninit();
  }

  static void init(void)
  {
    // sanity check
    assert(g_counter==0);
    assert(g_wasInitialized==false);
    assert(g_end==false);

    // TODO: prelude-init code goes here
  }

  static void uninit(void)
  {
    // sanity check
    assert(g_counter==0);
    assert(g_wasInitialized==true); // if we're here, prelude must have been initialized!

    try
    {
      // TODO: prelude-release code goes here
    }
    catch(...)
    {
      assert(!"exception thrown during prelude's deinitialization");
    }
  }
}; // struct Releaser

// this object will deiniti prelude uppon exit
Releaser g_rel;

} // unnamed namespace



GlobalLibPreludeInit::GlobalLibPreludeInit(void)
{
  System::Threads::SafeInitLock lock(g_mutex);
  if(g_wasInitialized==false)
  {
    Releaser::init();
    g_wasInitialized=true;
  }
  assert(g_wasInitialized==true);

  ++g_counter;
  assert(g_counter>0);
}

GlobalLibPreludeInit::~GlobalLibPreludeInit(void)
{
  System::Threads::SafeInitLock lock(g_mutex);
  assert(g_wasInitialized==true);
  assert(g_counter>0);
  --g_counter;

  // this is special condition, when Releaser instance is already dead
  // but some GlobalLibPreludeInit instance still existst and it has to
  // release it by itself
  if(g_counter==0 && g_end==true)
    Releaser::uninit();
}

} // namespace Prelude
} // namespace Input
