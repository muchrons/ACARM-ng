/*
 * Process.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_PROCESS_HPP_FILE
#define INCLUDE_PERSISTENCY_PROCESS_HPP_FILE

/* public header */

#include <string>

namespace Persistency
{

class Process
{
public:
  virtual ~Process(void);

  const std::string getPath(void) const;
  const std::string getName(void) const;
  const MD5Sum // TODO
protected:
private:
}; // class Process

} // namespace Persistency

#endif
