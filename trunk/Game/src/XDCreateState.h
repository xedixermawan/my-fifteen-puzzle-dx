/*
*  (c) 2013 XediXermawan < edi.ermawan@gmail.com >
*/
#ifndef __CREATE_STATE_H__
#define __CREATE_STATE_H__

#include "XDGameState.h"

class CreateState {
  public:
    static GameState* NewState(GameState::STATE state, XDGameRenderer* renderer);
    ~CreateState();
    CreateState() {};
};

#endif // __CREATE_STATE_H__