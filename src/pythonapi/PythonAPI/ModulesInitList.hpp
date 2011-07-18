/*
 * ModulesInitList.cpp
 *
 */
#ifndef INCLUDE_PYTHONAPI_MODULESINITLIST_HPP_FILE
#define INCLUDE_PYTHONAPI_MODULESINITLIST_HPP_FILE

#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/tuple/tuple.hpp>

#include "PythonAPI/ModuleInitFunction.hpp"

namespace PythonAPI
{

/** \brief colleciton holding user-provided python modules to be initialized.
 */
class ModulesInitList: private boost::noncopyable
{
private:
  typedef boost::tuple<const char*, ModuleInitFunction> ModuleInitSpec;
  typedef std::vector<ModuleInitSpec>                   ImportedModulesList;

public:
  /** \brief const iterator through elements of collection. */
  typedef ImportedModulesList::const_iterator const_iterator;

  /** \brief static call to schedule given module for importing.
   *  \param module module name (must be a compile-time string constant).
   *  \param init   init funtion (as defined by boost::python's macros).
   */
  void scheduleImport(const char *module, ModuleInitFunction init)
  {
    mods_.push_back( ModuleInitSpec(module, init) );
  }

  /** \brief get iterator to the collection's begin.
   */
  const_iterator begin(void) const
  {
    return mods_.begin();
  }
  /** \brief get iterator to the collection's end.
   */
  const_iterator end(void) const
  {
    return mods_.end();
  }
  /** \brief gets the number of schedules modules.
   */
  size_t count(void)
  {
    return mods_.size();
  }
  /** \brief clears collection's content.
   */
  void clear(void)
  {
    ImportedModulesList tmp;
    mods_.swap(tmp);
  }

private:
  ImportedModulesList mods_;
}; // class ModulesInitList

} // namespace PythonAPI

#endif
