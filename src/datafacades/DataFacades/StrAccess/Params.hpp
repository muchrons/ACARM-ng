/*
 * Params.hpp
 *
 */
#ifndef INCLUDE_DATAFACADES_STRACCESS_PARAMS_HPP_FILE
#define INCLUDE_DATAFACADES_STRACCESS_PARAMS_HPP_FILE

/* public header */

#include <cassert>
#include <boost/mpl/at.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/has_key.hpp>

#include "Commons/SharedPtrNotNULL.hpp"
#include "DataFacades/StrAccess/Path.hpp"
#include "DataFacades/StrAccess/SpecialMapKeys.hpp"

namespace DataFacades
{
namespace StrAccess
{

/** \brief tepresentation of paramters for main processing code (both runtime and compile-time).
 *  \param THandleMap      boost::mpl::map<> if handles for given situations/types.
 *  \param TResultCallback callback to use for reporting results and NULLs.
 */
template<typename THandleMap, typename TResultCallback>
struct Params
{
  /** \brief forwarding of map of handles paramter. */
  typedef THandleMap           HandleMap;
  /** \brief foraward of result callback type. */
  typedef TResultCallback      ResultCallback;
  /** \brief const-iterator made short. */
  typedef Path::const_iterator PathCIT;

private:
  /** \brief throws error in runtime on unknown handle in compile time via common API.
   */
  struct OnUnknownType
  {
    /** \brief process parameters for main processing code.
     *  \param p parameters for main processing code.
     *  \return false if not throw on invalid path.
     */
    template<typename T>
    static bool process(const T &/*e*/, const Params<THandleMap,TResultCallback> &p)
    {
      GetHandle<ErrorHandle>::type::throwOnInvalidPath(SYSTEM_SAVE_LOCATION, p);
      return false; // code never reaches here
    }
  }; // struct OnUnknownType

public:
  /** \brief creates partamters.
   *  \param path     path to be extracted.
   *  \param callback callback instance reference.
   */
  Params(const Path &path, ResultCallback &callback):
    path_(new Path(path)),
    now_(path_->begin()),
    callback_(&callback)
  {
    assert(path_.get()!=NULL);
    assert(callback_!=NULL);
  }

  /** \brief accessor for handles made easy.
   */
  template<typename T>
  struct GetHandle
  {
    /** \brief meta-program outputing handle for a given type/situation or one that given runtime error. */
    typedef typename boost::mpl::if_c< boost::mpl::has_key<HandleMap,T>::value,
                                     // then
                                       typename boost::mpl::at<HandleMap,T>::type,
                                     // else
                                       OnUnknownType
                                     >::type type;
  }; // struct GetHandle

  /** \brief pre-increment operator - moves params to next path elem.
   *  \return reference to this.
   */
  Params &operator++(void)
  {
    assert(!isEnd());
    ++now_;
    return *this;
  }

  /** \brief gets whole path.
   *  \return whole path as an object.
   */
  const Path &path(void) const
  {
    return *path_;
  }

  /** \brief gets current position in path.
   *  \return string with a name of current position in path.
   */
  const std::string &get(void) const
  {
    assert(!isEnd());
    return *now_;
  }

  /** \brief check for end of path.
   *  \return true, if end of path has been reached, false otherwise.
   */
  bool isEnd(void) const
  {
    return now_==path_->end();
  }

  /** \brief checks for existance of the next element in path.
   *  \return true if next element exists (i.e. ++(*this) is valid and will
   *          not result in end element), false otherwise.
   */
  bool hasNext(void) const
  {
    if(isEnd())
      return false;
    PathCIT tmp=now_;
    ++tmp;
    return tmp!=path_->end();
  }

  /** \brief gets access to result callback object.
   *  \return reference to object that can be used for callbacks.
   */
  ResultCallback &callback(void)
  {
    assert(callback_!=NULL);
    return *callback_;
  }

private:
  typedef Commons::SharedPtrNotNULL<const Path> PathPtr;

  // NOTE: using smart pointer instead of value is of a great importance here, since
  //       it does not require to trace iterators to a copyied value of path_.
  //       one value is just enough.
  //       another denefit is getting lightweight copy (pointer instead of the value).
  PathPtr         path_;
  PathCIT         now_;
  ResultCallback *callback_;
}; // struct Params

} // namespace StrAccess
} // namespace DataFacades

#endif
