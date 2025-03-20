#pragma once

enum class State
{
  STANDING,
  DUCKING,
  JUMPING,
  DIVING
};

class FiniteMachine
{
public:
  bool m_a = false;
  bool m_b = false;
  State m_currentState = State::STANDING;


  State getCurrentState();

  State changeState();
  
  void SetA();
  void SetB();
};