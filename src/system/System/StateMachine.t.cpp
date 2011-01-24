/*
 * StateMachine.t.cpp
 *
 * test set for StateMachine.
 *
 */
#include <tut.h>

#include "System/StateMachine.hpp"

using namespace System;

namespace
{

struct TestClass
{
  struct MyEnum
  {
    typedef enum
    {
      STATE_A=0,
      STATE_B=1,
      STATE_C=2
    } Type;
  };
  typedef System::Enum<MyEnum> StateEnum;

  struct StateA: public System::StateBase<StateEnum>
  {
    virtual StateEnum doStep(void)
    {
      return StateEnum::STATE_B;
    }
  };
  struct StateB: public System::StateBase<StateEnum>
  {
    StateB(void):
      cnt_(0)
    {
    }
    virtual StateEnum doStep(void)
    {
      ++cnt_;
      return StateEnum::STATE_C;
    }

    virtual void reset(void)
    {
      cnt_=0;
    }

    int cnt_;
  };
  struct StateC: public System::StateBase<StateEnum>
  {
    virtual StateEnum doStep(void)
    {
      return StateEnum::STATE_C;
    }
  };

  class TestStateMachine: public StateMachine<StateEnum>
  {
  public:
    TestStateMachine(void):
      StateMachine<StateEnum>(StateEnum::STATE_A, 3)
    {
      addState(StateEnum::STATE_A, &a);
      addState(StateEnum::STATE_B, &b);
      addState(StateEnum::STATE_C, &c);
    }

    int getStateBcnt(void) const
    {
      return b.cnt_;
    }

  private:
    StateA a;
    StateB b;
    StateC c;
  };


  // test data itself :)
  TestClass(void):
    vsm_(&sm_)
  {
  }

  TestStateMachine         sm_;
  StateMachine<StateEnum> *vsm_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;
factory tf("System/StateMachine");
}


namespace tut
{

// check default state
template<>
template<>
void testObj::test<1>(void)
{
  ensure( vsm_->currentState()==StateEnum::STATE_A );
}

// perform single stepping
template<>
template<>
void testObj::test<2>(void)
{
  ensure( vsm_->step()==StateEnum::STATE_B );
  ensure( vsm_->step()==StateEnum::STATE_C );
  ensure( vsm_->step()==StateEnum::STATE_C );
}

// reset machine in work
template<>
template<>
void testObj::test<3>(void)
{
  ensure( vsm_->step()==StateEnum::STATE_B );
  vsm_->reset();
  ensure( vsm_->step()==StateEnum::STATE_B );
}

// check if reseting state-objects works
template<>
template<>
void testObj::test<4>(void)
{
  ensure( sm_.getStateBcnt()==0 );

  ensure( vsm_->step()==StateEnum::STATE_B );
  ensure( sm_.getStateBcnt()==0 );

  ensure( vsm_->step()==StateEnum::STATE_C );
  ensure( sm_.getStateBcnt()==1 );

  ensure( vsm_->step()==StateEnum::STATE_C );
  ensure( sm_.getStateBcnt()==1 );

  vsm_->reset();
  ensure( sm_.getStateBcnt()==0 );
}

} // namespace tut

