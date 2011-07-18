/*
 * Environment.cpp
 *
 */
#include <vector>
#include <cassert>
#include <boost/tuple/tuple.hpp>

#include "System/AtExit.hpp"
#include "Base/EditableCString.hpp"
#include "Logger/Logger.hpp"
#include "Commons/computeHash.hpp"
#include "PythonAPI/Environment.hpp"
#include "PythonAPI/ExceptionHandle.hpp"

namespace py=boost::python;


namespace PythonAPI
{

namespace
{
/** \brief global flag for ensuring that environment has been initialized. */
bool g_alreadyInitialized=false;
} // unnamed namespace


/** \brief this is a work-around for a global initialization issue in Python's C interface.
 */
class Environment::ImportedModules
{
private:
  typedef boost::tuple<const char*, Environment::ModuleInitFunc> ModuleInitSpec;
  typedef std::vector<ModuleInitSpec>                            ImportedModulesList;

public:
  /** \brief static call to schedule given module for importing.
   *  \param module module name (must be a compile-time string constant).
   *  \param init   init funtion (as defined by boost::python's macros).
   */
  static void scheduleImport(const char *module, Environment::ModuleInitFunc init)
  {
    assert(!g_alreadyInitialized && "trying to import after initialization");
    get().push_back( ModuleInitSpec(module, init) );
  }

  /** \brief called after all registrations are done and registers all scheduled modules.
   */
  static void importAllModules(void)
  {
    assert(!g_alreadyInitialized && "trying to init/import all modules after initialization");
    // import all modules
    for(ImportedModulesList::const_iterator it=get().begin(); it!=get().end(); ++it)
      Environment::importModule( it->get<0>(), it->get<1>() );
    // all modeules imported - remove them from the collection
    ImportedModulesList tmp;
    get().swap(tmp);
  }

  /** \brief gets the number of schedules modules.
   */
  static size_t count(void)
  {
    return get().size();
  }

private:
  ImportedModules(void);    // no instances allowed

  // global collection, initialized uppon first usage
  static ImportedModulesList &get(void)
  {
    static ImportedModulesList mods;
    return mods;
  }
}; // class ImportedModules



Environment::StaticImporter::StaticImporter(const char *module, ModuleInitFunc init)
{
  // sanity check
  assert(module!=NULL);
  assert(init!=NULL);
  assert(imported_==false);
  // schedule import
  const Logger::Node log("pythonapi.environment.staticimporter");
  LOGMSG_INFO_S(log)<<"scheduling module '"<<module<<"' for importing";
  ImportedModules::scheduleImport(module, init);
  imported_=true;
}

Environment::StaticImporter::~StaticImporter(void)
{
  assert(imported_==true);
}



Environment::Environment(void):
  log_("pythonapi.environment")
{
  LOGMSG_DEBUG(log_, "initializing Python's environment object");
  // if called for the first time import all modules
  if(!g_alreadyInitialized)
  {
    // modules importing
    const size_t count=ImportedModules::count();
    LOGMSG_INFO_S(log_)<<"importing all of the "<<count<<" registered modules";
    ImportedModules::importAllModules();
    LOGMSG_INFO_S(log_)<<"all of the "<<count<<" registered modules imported";
    // python's init
    Py_Initialize();
    // done
    g_alreadyInitialized=true;
    LOGMSG_INFO(log_, "initializing Python's envioronment succeeded");
  }
  // casual stuff
  mainModule_   =py::import("__main__");
  mainNamespace_=mainModule_.attr("__dict__");
  LOGMSG_DEBUG(log_, "initializing Python's envioronment object succeeded");
}

void Environment::run(const std::string &script)
{
  assert(g_alreadyInitialized);
  LOGMSG_DEBUG_S(log_)<<"running script; script's hash is: "<<Commons::computeHash(script);
  try
  {
    ExceptionHandle::clearState();
    py::exec(script.c_str(), mainNamespace_);
  }
  catch(const boost::python::error_already_set &)
  {
    LOGMSG_ERROR_S(log_)<<"exception from script (script's hash is "<<Commons::computeHash(script)<<")";
    ExceptionHandle ex;
    LOGMSG_ERROR_S(log_)<<"exception is: "<<ex.str();
    ex.rethrow();
  }
}


namespace
{
class StringHolder: public System::AtExitResourceDeallocator
{
public:
  explicit StringHolder(const char *str):
    str_(str)
  {
  }

  virtual void deallocate(void)
  {
    Base::EditableCString tmp("");
    str_.swap(tmp);
  }

  char *get(void)
  {
    return str_.get();
  }

private:
  Base::EditableCString str_;
}; // class StringHolder
} // unnamed namespace


void Environment::importModule(const char *module, ModuleInitFunc init)
{
  assert(!g_alreadyInitialized && "trying to import module when environment has been already initialized");

  const Logger::Node log("pythonapi.environment");
  LOGMSG_INFO_S(log)<<"importing module '"<<module<<"' with init function at: "<<std::hex<<reinterpret_cast<void*>(init);

  if(module==NULL)
    throw Exception(SYSTEM_SAVE_LOCATION, "module name pointer cannot be NULL");
  if(init==NULL)
    throw Exception(SYSTEM_SAVE_LOCATION, "init function cannot be NULL");

  // this voodoo ensures that string will be valid as long as main() doesn't exit
  StringHolder                *str=new StringHolder(module);
  System::AtExit::TDeallocPtr  ptr(str);
  System::AtExit::registerDeallocator(ptr);
  assert(str!=NULL);
  if( PyImport_AppendInittab(str->get(), init)!=0 )
    throw Exception(SYSTEM_SAVE_LOCATION, std::string("PyImport_AppendInittab() failed; unable to init module: ")+module);

  LOGMSG_INFO_S(log)<<"module '"<<module<<"' imported successfuly";
}

void Environment::importModule(const std::string &module)
{
  assert(g_alreadyInitialized);
  run("import "+module);
}

} // namespace PythonAPI
