/*
 * AnalyzersMap.hpp
 *
 */
#ifndef INCLUDE_INPUT_ANALYZERSMAP_HPP_FILE
#define INCLUDE_INPUT_ANALYZERSMAP_HPP_FILE

/* public header */

#include <map>
#include <string>
#include <boost/noncopyable.hpp>

#include "Persistency/Analyzer.hpp"
#include "Input/Exception.hpp"

// TODO

namespace Input
{

/** \brief main mapping object - maps input's IDs to objects.
 */
class AnalyzersMap: private boost::noncopyable
{
public:
  struct ExceptionAddingAlreadyExistingKey: public Exception
  {
    ExceptionAddingAlreadyExistingKey(const Location                   &where,
                                      const std::string                &id,
                                      const Persistency::AnalyzerPtrNN  analyzer):
      Exception(where, cc("trying to add duplicate entry - input ID '",
                          id, "', analyzer '", analyzer->getName().get(), "'") )
    {
    }
  }; // struct ExceptionAddingAlreadyExistingKey

  /** \brief gets mapping from given input's ID to analyzer. if ID's not mapped NULL is retunred.
   *  \param id ID returned by input (mixed with input name).
   *  \return analyzer's object instance or NULL.
   */
  Persistency::AnalyzerPtrNN::SharedPtr get(const std::string &id) const;
  /** \brief adds new mapping.
   *  \param id      id to map from (key).
   *  \param analzer analyzer to map to (value).
   */
  void add(const std::string &id, Persistency::AnalyzerPtrNN analyzer);

private:
  typedef std::map<std::string, Persistency::AnalyzerPtrNN> InputIDMap;

  InputIDMap map_;
}; // class AnalyzersMap

} // namespace Input

#endif
