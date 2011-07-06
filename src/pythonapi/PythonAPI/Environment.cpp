/*
 * Environment.cpp
 *
 */
#include <vector>
#include <cassert>
#include <boost/tuple.hpp>

#include "Logger/Logger.hpp"
#include "Commons/computeHash.hpp"
#include "PythonAPI/Environment.hpp"
#include "PythonAPI/ExceptionHandle.hpp"

namespace py=boost::python;


namespace PythonAPI
{

namespace
{
bool g_alreadyInitialized=false;

// this is a work-around for a global initialization issue in Python's C interface
class ImportedModules
{
private:
  typedef boost::tuple<std::string, Environment::ModuleInitFunc> ModuleInitSpec;
  typedef std::vector<ModuleInitSpec>                            ImportedModules;

public:
  static void scheduleImport(const char *module, ModuleInitFunc init)
  {
    assert(!g_alreadyInitialized && "trying to import after initialization");
    get().push_back( ModuleInitSpec(module, init) );
  }

  static void importAllModules(void)
  {
    assert(!g_alreadyInitialized && "trying to init/import all modules after initialization");
    // import all modules
    for(ImportedModules::const_iterator it=get().begin(); it!=get().end(); ++it)
      Environment::importModule( it->get<0>(), it->get<1>() );
    // all modeules imported - remove them from the collection
    ImportedModules tmp;
    get().swap(tmp);
  }

  static size_t count(void)
  {
    return get().size();
  }

private:
  ImportedModules(void);    // no instances allowed

  // global collection, initialized uppon first usage
  static ImportedModules &get(void)
  {
    static ImportedModules mods;
    return mods;
  }
}; // class ImportedModules
} // unnamed namespace


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
  LOGMSG_INFO_S(log_)<<"initializing Python's envioronment with "<<g_importsCount<<" imported modules";
  // if called for the first time import all modules
  if(!g_alreadyInitialized)
  {
    // modules importing
    LOGMSG_INFO_S(log_)<<"importing all of the "<<ImportedModules::count()<<" registered modules";
    ImportedModules::importAllModules();
    LOGMSG_INFO_S(log_)<<"all of the "<<ImportedModules::count()<<" registered modules imported";
    // python's init
    Py_Initialize();
    // done
    g_alreadyInitialized=true;
  }
  // casual stuff
  mainModule_   =py::import("__main__");
  mainNamespace_=mainModule_.attr("__dict__");
  LOGMSG_INFO(log_, "initializing Python's envioronment succeeded");
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

void Environment::importModule(const std::string &module, ModuleInitFunc init)
{
  assert(!g_alreadyInitialized && "trying to import module when environment has been already initialized");

  const Logger::Node log("pythonapi.environment");
  LOGMSG_INFO_S(log)<<"importing module '"<<module<<"' with init function at: "<<std::hex<<reinterpret_cast<void*>(init);

  if(init==NULL)
    throw Exception(SYSTEM_SAVE_LOCATION, "init function cannot be NULL");

  if( PyImport_AppendInittab(module.c_str(), init)!=0 )
    throw Exception(SYSTEM_SAVE_LOCATION, "PyImport_AppendInittab() failed; unable to init module "+module);

  ++g_importsCount;
  LOGMSG_INFO_S(log)<<"module '"<<module<<"' imported successfuly";
}

} // namespace PythonAPI
