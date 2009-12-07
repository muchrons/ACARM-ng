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
 *  this class should be deriven from. handlers
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
    _resetState  ( startState ),
    _currentState( _resetState  ),
    _states      ( statesCount )
  {
    assert( _states.size()==statesCount );
    // reset whole state machine
    for(typename StateCollection::iterator it=_states.begin();
        it!=_states.end();
        ++it)
      *it=NULL;
  }

  /** \brief reset state machine to primary state.
   */
  inline void reset(void)
  {
    // set start state.
    _currentState=_resetState;
    // reset internal states of each object.
    for(typename StateCollection::iterator it=_states.begin();
        it!=_states.end();
        ++it)
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
    return _currentState;
  }

  /** \brief perform single step of state machine.
   *  \return current state SM is in.
   */
  inline TEnum step(void)
  {
    // get current state from map:
    assert( _currentState.toInt()>=0 );
    const unsigned long curSt=_currentState.toInt();
    assert( /* 0<=curSt && */ curSt<_states.size() );
    StateType *se=_states[curSt];
    assert( se!=NULL );
    _currentState=se->doStep();     // perform step to next one
    return _currentState;           // return state reached
  }

protected:
  /** \brief add state to state-set.
   *
   *  this should be used by derived class to add
   *  handlers for a given states. it will not allow to pass
   *  incorrect values to state-set.
   *
   *  \param state   state to be added.
   *  \param handler object to handle given state. ownership must be
   *         held in derived class throught whole life-cycle. it
   *         is quaratneed that pointers will not be used in
   *         destructor.
   */
  inline void addState(const TEnum state, StateType *handler)
  {
    assert( state.toInt()>=0 );
    const unsigned long stateInt=state.toInt();
    assert( /* 0<=stateInt && */ stateInt<_states.size() ||
            !"index out of bounds - possibly number of enries is too small" );
    assert( _states[stateInt]==NULL ||
            !"given state is already handled");     // no overwriting
    _states[stateInt]=handler;
  }

private:
  const TEnum     _resetState;
  TEnum           _currentState;
  StateCollection _states;
}; // class StateMachine

} // namespace System

#endif

