#include "FiniteMachine.h"

State FiniteMachine::getCurrentState()
{
  return m_currentState;
}

State FiniteMachine::changeState()
{
  switch (m_currentState)
  {
    case State::STANDING:
    {
      if(m_a)
      {
        m_currentState = State::DUCKING;
      }
      else if (m_b)
      {
        m_currentState = State::JUMPING;
      }
    }
      break;
    case State::DUCKING:
    {
      if (!m_a)
      {
        m_currentState = State::STANDING;
      }
    }
      break;
    case State::JUMPING:
    {
      if (m_a)
      {
        m_currentState = State::DIVING;
      }
    }
      break;
    case State::DIVING:
      break;
    default:
      break;
  }
  return m_currentState;
}



void FiniteMachine::SetA()
{
  m_a = !m_a;
}

void FiniteMachine::SetB()
{
  m_b = !m_b;
}
