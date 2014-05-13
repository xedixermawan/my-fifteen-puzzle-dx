/*
*  (c) 2013 XediXermawan < edi.ermawan@gmail.com >
*/

#ifndef __STATE_LOADING_H__
#define __STATE_LOADING_H__

#include "XDGameState.h"
#include <chrono>

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::steady_clock;

class StateLoading : public GameState {
  public:
    StateLoading(XDGameRenderer* renderer);
    void Initialize();
    int Update(const double delta_time);
    void Render();
    void RenderDebug();
    void OnSuspending();
    void OnResuming(XDGameRenderer* renderer);
    void OnHandleInput(  const SINPUT& input );
    STATE Next();
    ~StateLoading();
  private:
    std::shared_ptr< XDSprite >  m_SprLoading;
    std::shared_ptr< XDAnimObject >  m_AnimLoading;
    steady_clock::time_point m_StateStartTime;
};
#endif  // __STATE_LOADING_H__