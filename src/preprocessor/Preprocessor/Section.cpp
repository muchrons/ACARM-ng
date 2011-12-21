/*
 * Section.cpp
 *
 */
#include <cassert>

#include "DataFacades/StrAccess/StrAccess.hpp"
#include "Preprocessor/Section.hpp"
#include "Preprocessor/Expressions/And.hpp"
#include "Preprocessor/Expressions/Or.hpp"
#include "Preprocessor/Expressions/Not.hpp"
#include "Preprocessor/Expressions/Constant.hpp"
#include "Preprocessor/Expressions/Rule.hpp"

namespace Preprocessor
{

Section::Section(const ConfigIO::Preprocessor::Section &cfg):
  isAcceptType_( cfg.getType()==ConfigIO::Preprocessor::Section::Type::ACCEPT ),
  expr_( create( cfg.getExpression() ) )
{
  assert( expr_.get()!=NULL );
}

Section::Decision Section::process(const Persistency::ConstGraphNodePtrNN &node) const
{
  assert( expr_.get()!=NULL );
  const bool result=expr_->compute(node);

  // accept-section returned true
  if( isAcceptType_==true && result==true )
    return Decision::ACCEPT;

  // reject-section returned true
  if( isAcceptType_==false && result==true )
    return Decision::REJECT;

  // if not, it means that result cannot be determined at this level
  return Decision::CONTINUE;
}

Expressions::Set Section::create(const ConfigIO::Preprocessor::Expression::Expressions &cfg) const
{
  assert( cfg.size()>0 );

  Expressions::Set out;
  out.reserve( cfg.size() );
  // loop throught each element
  for(ConfigIO::Preprocessor::Expression::Expressions::const_iterator it=cfg.begin(); it!=cfg.end(); ++it)
    out.push_back( create(*it) );

  assert( out.size()>0 );
  return out;
}

Expressions::BasePtrNN Section::create(const ConfigIO::Preprocessor::Expression &cfg) const
{
  switch( cfg.getType().toInt() )
  {
    case ConfigIO::Preprocessor::Expression::Type::AND:
      {
        assert( cfg.getRules().size()==0 );
        return Expressions::BasePtrNN( new Expressions::And( create( cfg.getExpressions() ) ) );
      }

    case ConfigIO::Preprocessor::Expression::Type::OR:
      {
        assert( cfg.getRules().size()==0 );
        return Expressions::BasePtrNN( new Expressions::Or( create( cfg.getExpressions() ) ) );
      }

    case ConfigIO::Preprocessor::Expression::Type::NOT:
      {
        assert( cfg.getRules().size()==0 );
        const Expressions::Set tmp=create( cfg.getExpressions() );
        assert( tmp.size()==1 );
        return Expressions::BasePtrNN( new Expressions::Not( tmp.at(0) ) );
      }

    case ConfigIO::Preprocessor::Expression::Type::TERM:
      {
        assert( cfg.getExpressions().size()==0 );
        assert( cfg.getRules().size()==1 );
        return create( cfg.getRules().at(0) );
      }

    default:
      assert(!"unknown expression type - update the code");
  } // switch(type)

  // we never reach here
  return Expressions::BasePtrNN( new Expressions::False() );
}

Expressions::BasePtrNN Section::create(const ConfigIO::Preprocessor::Rule &cfg) const
{
  switch( cfg.getType().toInt() )
  {
    case ConfigIO::Preprocessor::Rule::Type::RULE_TRUE:

      return Expressions::BasePtrNN( new Expressions::True() );

    case ConfigIO::Preprocessor::Rule::Type::RULE_FALSE:
      return Expressions::BasePtrNN( new Expressions::False() );

    case ConfigIO::Preprocessor::Rule::Type::RULE:
      typedef DataFacades::StrAccess::Path Path;
      return Expressions::BasePtrNN( new Expressions::Rule( Path(cfg.getPath()),
                                                            cfg.getMode(),
                                                            cfg.getValue(),
                                                            cfg.getFormatter() ) );

    default:
      assert(!"unknown rule type - update the code");
  } // switch(type)

  // we never reach here
  return Expressions::BasePtrNN( new Expressions::False() );
}

} // namespace Preprocessor
