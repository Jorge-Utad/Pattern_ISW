#include <Windows.h>
#include <cstdio>
#include "lib/FiniteMachine.h"
#pragma comment (lib, "user32")
#define GetKey(X) (!!GetAsyncKeyState(0[#X])&0x8000)

int main()
{
  FiniteMachine fm;
  printf("Curent State: %d\n", fm.getCurrentState());
  
  fm.SetA();
  fm.changeState();
  printf("Curent State: %d\n", fm.getCurrentState());

  fm.SetA();
  fm.changeState();
  printf("Curent State: %d\n", fm.getCurrentState());

  fm.SetB();
  fm.changeState();
  printf("Curent State: %d\n", fm.getCurrentState());

  fm.SetA();
  fm.changeState();
  printf("Curent State: %d\n", fm.getCurrentState());
}