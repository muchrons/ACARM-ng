/*
 * PersistencyProxy.hpp
 *
 */
#ifndef INCLUDE_INPUT_PERSISTENCYPROXY_HPP_FILE
#define INCLUDE_INPUT_PERSISTENCYPROXY_HPP_FILE

/* public header */

#include <string>
#include <boost/noncopyable.hpp>

#include "Persistency/Analyzer.hpp"

namespace Input
{

/** \brief object to forward mapping saving to persistency.
 */
struct PersistencyProxy
{
  /** \brief ensure proper, polymorphic destruction.
   */
  virtual ~PersistencyProxy(void);

  /** \brief request to save mapping inputID->id.
   *  \param inputID key of mapping.
   *  \param id      value of mapping.
   */
  virtual void saveMapping(const std::string &inputID, Persistency::Analyzer::ID id) = 0;

  /** \brief read next free ID.
   *  \return ID saved as a last free one.
   */
  virtual Persistency::Analyzer::ID readNextFreeID(void) = 0;
  /** \brief save next free ID.
   *  \param nextFreeID free ID to be saved.
   */
  virtual void saveNextFreeID(Persistency::Analyzer::ID nextFreeID) = 0;
}; // struct PersistencyProxy

} // namespace Input

#endif
