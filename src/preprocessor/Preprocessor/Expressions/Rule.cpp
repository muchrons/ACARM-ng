/*
 * Rule.cpp
 *
 */
#include <memory>
#include <cassert>
#include <boost/mpl/insert.hpp>

#include "Commons/Convert.hpp"
#include "Preprocessor/Checkers/Equals.hpp"
#include "Preprocessor/Checkers/LessThan.hpp"
#include "Preprocessor/Checkers/GreaterThan.hpp"
#include "Preprocessor/Checkers/Contains.hpp"
#include "Preprocessor/Checkers/RegExp.hpp"
#include "Preprocessor/Expressions/Rule.hpp"
#include "Preprocessor/Expressions/FormatterBuilder.hpp"

using namespace std;
using namespace DataFacades::StrAccess;


namespace Preprocessor
{
namespace Expressions
{

namespace
{

/** \brief callback mechanism for StrAccess.
 */
struct CallbackHandle
{
  /** \brief construyct instance with a given checker.
   *  \param checker checker to be run for each callback.
   *  \param value   value to be assigned to the formatter.
   *  \param fmt     formatter to be used for data processing.
   */
  CallbackHandle(Checkers::Mode *checker, Formatters::ValuePtrNN value, Formatters::BasePtrNN fmt):
    checker_(checker),
    value_(value),
    fmt_(fmt)
  {
    assert(checker_!=NULL);
  }

  /*
  NOTE: this call is never used by the implementation!
  bool collectionSize(size_t size);
  */

  /** \brief callback of specific value.
   *  \param v value received.
   */
  bool value(const std::string &v)
  {
    return checkAgainst(v);
  }

  /** \brief callback called on NULL pointer in the path.
   */
  bool nullOnPath(const std::string &/*where*/)
  {
    return checkAgainst("<NULL>");
  }

private:
  bool checkAgainst(const std::string &str)
  {
    // format parameter
    value_->set(str);
    const std::string &out=fmt_->exec();
    // pass it to the final checker
    assert(checker_!=NULL);
    return checker_->check(out);
  }

  Checkers::Mode         *checker_;
  Formatters::ValuePtrNN  value_;
  Formatters::BasePtrNN   fmt_;
}; // struct CallbackHandle


/** \brief processing facility handling collection indexes.
 */
struct OnCollectionStrategy: private System::NoInstance
{
  /** \brief processing method.
   *  \param e element to be processed.
   *  \param p params to be used when processing.
   *  \return value farwarded from further user's calls.
   */
  template<typename T, typename TParams>
  static bool process(const T &e, TParams p)
  {
    BOOST_STATIC_ASSERT( IsCollection<T>::value );
    assert(!p.isEnd());
    typedef typename TParams::template GetHandle<ErrorHandle>::type ErrH;

    if(p.get()=="$")
      return matchAny(e, p);
    if(p.get()=="*")
      return matchAll(e, p);

    // invalid element type
    ErrH::throwOnInvalidIndex(SYSTEM_SAVE_LOCATION, p);
    return false;   // this code is never reached
  }

private:
  template<typename T, typename TParams>
  static bool matchAll(const T &e, TParams p)
  {
    return matchImpl(e, p, false);
  }

  template<typename T, typename TParams>
  static bool matchAny(const T &e, TParams p)
  {
    return matchImpl(e, p, true);
  }

  // common implementation for matchAll/matchAny
  template<typename T, typename TParams>
  static bool matchImpl(const T &e, TParams p, const bool exp)
  {
    if(e.begin()==e.end())
      return false;
    for(typename T::const_iterator cit=e.begin(); cit!=e.end(); ++cit)
      if( MainDispatcher::process(*cit, p)==exp )
        return exp;
    return !exp;
  }
}; // struct OnCollectionStrategy


/** \brief helper class that overwrites default error handling for collection indexes.
 *
 *  default collection can have number as an accessing token. here only '$' and '*'
 *  are allowed. everything else is forbidden.
 */
struct ErrorThrowerMod: public ErrorThrower
{
  /** \brief throws exception if element's name is invalid and is not and collection-part token.
   *  \param where location where condition is checked.
   *  \param name  expected name in the path.
   *  \param p     current params value.
   */
  template<typename TParams>
  static void throwOnInvalidName(const ExceptionInvalidPath::Location &where, const TParams &p, const std::string &name)
  {
    typedef typename boost::mpl::at<typename TParams::HandleMap, InvalidPathExceptionType>::type ExceptionType;
    if( p.get()!=name && p.get()!="$" && p.get()!="*" )
      throw ExceptionType(where, p.path().get(), p.get(), "invalid name/collection-part-token ($/*) in path");
  }
}; // struct ErrorThrowerMod


/** \brief helper funciton building proper mode checker.
 *  \param mode  mode of checking to construct.
 *  \param value value to be comapred with.
 *  \return newly constructed mode checker.
 */
std::auto_ptr<Checkers::Mode> buildChecker(Rule::Mode mode, const Rule::Value &value)
{
  typedef std::auto_ptr<Checkers::Mode> Ptr;
  // create proper checker
  switch( mode.toInt() )
  {
    case Rule::Mode::EQUALS:      return Ptr( new Checkers::Equals(value) );
    case Rule::Mode::LESSTHAN:    return Ptr( new Checkers::LessThan(value) );
    case Rule::Mode::GREATERTHAN: return Ptr( new Checkers::GreaterThan(value) );
    case Rule::Mode::CONTAINS:    return Ptr( new Checkers::Contains(value) );
    case Rule::Mode::REGEXP:      return Ptr( new Checkers::RegExp(value, true) );
    case Rule::Mode::REGEXPCI:    return Ptr( new Checkers::RegExp(value, false) );
    default:                      break;
  } // switch(mode)

  assert(!"unknown mode requested");
  throw std::logic_error("requested unknown mode of comparison - code is NOT updated");
  return Ptr(NULL); // never reached
} // buildChecker()


Formatters::BasePtrNN buildFormatter(Formatters::ValuePtrNN value, const ConfigIO::Preprocessor::FormatterConfig &fmt)
{
  FormatterBuilder fb(value);
  return fb.build(fmt);
} // buildFormatter()

} // unnamed namespace



Rule::Rule(const Path &path, Mode mode, const Value &value, const ConfigIO::Preprocessor::FormatterConfig &fmt):
  path_(path),
  checker_( buildChecker(mode, value).release() ),
  formatterValue_(new Formatters::Value),
  baseFormatter_( buildFormatter(formatterValue_, fmt) )
{
  assert( checker_.get()!=NULL );
}


bool Rule::compute(const Persistency::ConstGraphNodePtrNN &node) const
{
  // for preprocessor we use standard handle map, except for changed collection
  // handle to the one supporting wildcards.
  typedef boost::mpl::insert<DefaultHandleMap,
                             boost::mpl::pair<CollectionIndexHandle,OnCollectionStrategy>::type
                            >::type CollectionStrategyHandleMap;
  // use standard map, with overwritten some of the error handling mechanisms
  typedef boost::mpl::insert<CollectionStrategyHandleMap,
                             boost::mpl::pair<ErrorHandle,ErrorThrowerMod>::type
                            >::type ErrorModHandleMap;
  // use local exception to throw errors
  typedef boost::mpl::insert<ErrorModHandleMap,
                             boost::mpl::pair<InvalidPathExceptionType,Preprocessor::ExceptionInvalidPath>::type
                            >::type ExceptionTypeHandleMap;
  // define final handle map
  typedef ExceptionTypeHandleMap PreprocHandleMap;

  typedef Params<PreprocHandleMap, CallbackHandle> ParamsImpl;
  CallbackHandle cb(checker_.get(), formatterValue_, baseFormatter_);
  ParamsImpl     p(path_, cb);
  return MainDispatcher::process(node, p);
}

} // namespace Expressions
} // namespace Preprocessor
