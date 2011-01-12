/*
 * StateMachine.hpp
 *
 * easy to use and implement general purpose state
 * machine implementation.
 *
 */
#ifndef INCLUDE_SYSTEM_STATEMACHINE_HPP_FILE
#define INCLUDE_SYSTEM_STATEMACHINE_HPP_FILE

/* public header */

#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <cassert>

#include "System/Enum.hpp"
#include "System/Exception.hpp"


namespace System
{

/** \brief base class for all states.
 *
 *  object holds representation of single state for
 *  state machine. to bypass limitations of 'classical'
 *  state machines it has been decided to allow each
 *  object to have its internal state. this is very useful
 *  in case of states that would normaly represent just
 *  counting input signals, etc...
 */
template<typename TEnum>
struct StateBase
{
public:
  /** \brief make destructor virtual.
   */
  virtual ~StateBase(void)
  {
  }

  /** \brief step from this state to another.
   *  \return next state enum after call.
   */
  virtual TEnum doStep(void) = 0;

  /** \brief resetes internal state of object.
   *
   *  since 'classical' states should not contain
   *  state by itself, default implementation should be
   *  used as often as it is possible. before you'll
   *  add state-object's internal state think twice.
   */
  virtual void reset(void)
  {
  }
}; // struct StateBase


/** \brief class implementing the state machine itself.
 *
 *  this class should be deriven from. handles
 *  for ALL states must be given in constructor
 *  of derived class via addState() method.
 *  notice that each enum must have uniq value,
 *  first one starting with 0 and last one with n-1
 *  where n is number of states.
 */
template<typename TEnum>
class StateMachine: private boost::noncopyable
{
public:
  /** \brief type of state base class.
   */
  typedef StateBase<TEnum>        StateType;
  /** \brief type of contatiner with state pointers.
   */
  typedef std::vector<StateType*> StateCollection;

  /** \brief creates new state machine.
   *  \param startState state from which state machine should start.
   *  \param statesCount number of states to be used. this paramters
   *         allows both increased performacne and simpe user-checking
   *         (is number of added states matches).
   */
  StateMachine(const TEnum startState, const size_t statesCount):
    resetState_  ( startState ),
    currentState_( resetState_  ),
    states_      ( statesCount )
  {
    assert( states_.size()==statesCount );
    // reset whole state machine
    for(typename StateCollection::iterator it=states_.begin(); it!=states_.end(); ++it)
      *it=NULL;
  }

  /** \brief reset state machine to primary state.
   */
  inline void reset(void)
  {
    // set start state.
    currentState_=resetState_;
    // reset internal states of each object.
    for(typename StateCollection::iterator it=states_.begin(); it!=states_.end(); ++it)
    {
      assert((*it)!=NULL);
      (*it)->reset();
    }
  }

  /** \brief returns state in which machine is in.
   *  \return current state.
   */
  inline TEnum currentState(void) const
  {
    return currentState_;
  }

  /** \brief perform single step of state machine.
   *  \return current state SM is in.
   */
  inline TEnum step(void)
  {
    // get current state from map:
    assert( currentState_.toInt()>=0 );
    const unsigned long curSt=currentState_.toInt();
    assert( /* 0<=curSt && */ curSt<states_.size() );
    StateType *se=states_[curSt];
    assert( se!=NULL );
    currentState_=se->doStep();     // perform step to next one
    return currentState_;           // return state reached
  }

protected:
  /** \brief add state to state-set.
   *
   *  this should be used by derived class to add
   *  handles for a given states. it will not allow to pass
   *  incorrect values to state-set.
   *
   *  \param state  state to be added.
   *  \param handle object to handle given state. ownership must be
   *         held in derived class throught whole life-cycle. it
   *         is quaratneed that pointers will not be used in
   *         destructor.
   */
  inline void addState(const TEnum state, StateType *handle)
  {
    assert( state.toInt()>=0 );
    const unsigned long stateInt=state.toInt();
    assert( /* 0<=stateInt && */ stateInt<states_.size() ||
            !"index out of bounds - possibly number of enries is too small" );
    assert( states_[stateInt]==NULL ||
            !"given state is already handled");     // no overwriting
    states_[stateInt]=handle;
  }

private:
  const TEnum     resetState_;
  TEnum           currentState_;
  StateCollection states_;
}; // class StateMachine

} // namespace System

#endif

