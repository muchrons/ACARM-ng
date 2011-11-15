/*
 * CmdLineArgs.hpp
 *
 */
#ifndef INCLUDE_COMMONS_CMDLINEARGS_HPP_FILE
#define INCLUDE_COMMONS_CMDLINEARGS_HPP_FILE

/* public header */

#include <vector>
#include <boost/shared_array.hpp>
#include <boost/scoped_array.hpp>

#include "Commons/Exception.hpp"


namespace Commons
{

/** \brief class making copy of command line arguments.
 *
 *  this funcitonality is usefull when coping with classes that are not const-correct.
 */
class CmdLineArgs
{
public:
  /** \brief exception thrown when invalid argument is passed.
   */
  struct ExceptionInvalidArgument: public Exception
  {
    /** \brief create exception instance.
     *  \param where location of exception rising.
     *  \param what  exact error message.
     */
    ExceptionInvalidArgument(const Location &where, const char *what);
  }; // struct ExceptionInvalidArgument

  /** \brief creates local copy of the const-correct arguments.
   *  \param argc argument count.
   *  \param argv arguments array.
   */
  CmdLineArgs(int argc, char const * const * argv);

  /** \brief gives access to non-const array of arguments.
   *  \note array, as well as all it's elements, are ensured to be not NULL.
   *  \return returns array of pointers to arguments, compatible with main's argv.
   */
  char **argv(void) const;
  /** \brief arguments count (i.e. size of argv() array).
   *  \note returned value is always >=1.
   *  \return returns number of arguments, compatible with main's argc.
   */
  int argc(void) const;

private:
  int                                      argc_;
  // NOTE: this can be replaced by scoped_array<> with strings concatenated. this is
  //       not criticial part of code when it comes to memory/computation time,
  //       and so this soultion (as a simpler one) can stay.
  std::vector< boost::shared_array<char> > argvElems_;
  boost::scoped_array<char*>               argv_;
}; // class CmdLineArgs

} // namespace Commons

#endif
