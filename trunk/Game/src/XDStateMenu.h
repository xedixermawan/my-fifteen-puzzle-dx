/*
*  (c) 2013 XediXermawan < edi.ermawan@gmail.com >
*/
#ifndef __STATE_MENU_H__
#define __STATE_MENU_H__

#include "XDGameState.h"
#include "XDFiveTeenBox.h"
#include "XDButton.h"

class StateMenu : public GameState {
  public:
    StateMenu(XDGameRenderer* renderer);
    void Initialize();
    int Update(const double delta_time);
    void Render();
    void RenderDebug();
    void OnSuspending();
    void OnResuming(XDGameRenderer* renderer);
    void OnHandleInput(  const SINPUT& input );
    GameState::STATE Next();
    ~StateMenu();
  private:
    std::shared_ptr< XDSprite >  m_SprMenuText;
    std::shared_ptr< XDSprite >  m_SprMenuBg;
    std::shared_ptr< XDAnimObject >  m_AnimTextPlay;
    std::shared_ptr< XDAnimObject >  m_AnimTextOption;
    std::shared_ptr< XDAnimObject >  m_AnimTextExit;

    int m_mousex , m_mousey;
    XMFLOAT2 m_StartPoint;
    XMFLOAT2 m_WHCell;
    XMFLOAT2 m_WHBoard;

    PhysicTest* m_World;
    bool m_Sound;
    int m_BackKeyOnIGM_Count;
    std::shared_ptr < gui::Button > m_ButtonPlay;
    std::shared_ptr < gui::Button > m_ButtonOpt;
    std::shared_ptr < gui::Button > m_ButtonExit;
    std::shared_ptr < gui::Button > m_ButtonInfo;
    std::shared_ptr < gui::Button > m_ButtonSound;
    void ButtonPlayCB(void* sender);
    void ButtonOptionCB(void* sender);
    void ButtonExitCB(void* sender);
    void ButtonInfoCB(void* sender);
    void ButtonSoundCB(void* sender);
};
#endif  // __STATE_MENU_H__