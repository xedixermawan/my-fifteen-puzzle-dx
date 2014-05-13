/*
*  (c) 2013 XediXermawan < edi.ermawan@gmail.com >
*/
#include "pch.h"
#include "XDStateMenu.h"
#include "XDGUIManager.h"
#include <iostream>

extern float g_screen_w;
extern float g_screen_h;

StateMenu::StateMenu(XDGameRenderer* renderer) : GameState(renderer)  {
    m_NextState = STATE::EXIT;
}

void StateMenu::Initialize() {
    m_Sound = true;
    m_BackKeyOnIGM_Count=0;
    m_SprMenuText = std::shared_ptr< XDSprite> ( new XDSprite (std::string("five.xml") ) ) ;
    m_SprMenuText->SetSpriteBatch ( m_Renderer->SpriteBatchs[ XDGameRenderer::EBATCH::L0 ] );
    m_SprMenuText->LoadTexture( m_Renderer->GetDevice() );
    //m_SprMenuBg = std::shared_ptr< XDSprite> ( new XDSprite (std::string("menubg.xml") ) ) ;
    //m_SprMenuBg->SetSpriteBatch ( m_Renderer->SpriteBatchs[ XDGameRenderer::EBATCH::L0 ] );
    //m_SprMenuBg->LoadTexture( m_Renderer->GetDevice() );
    m_AnimTextPlay = std::shared_ptr< XDAnimObject> (  new XDAnimObject( m_SprMenuText.get() , 10 , 10 ) );
    m_AnimTextPlay->SetAnim(24);
    m_AnimTextPlay->SetPos(40,-10);
    m_ButtonPlay = std::shared_ptr < gui::Button > ( new gui::Button( m_SprMenuText ) );
    m_ButtonPlay->SetAnim(15);
    m_ButtonPlay->SetPos(140,200);
    m_ButtonPlay->SetWidthHeight(200,50);
    gui::GUIManager::GetInstance().AddElement( m_ButtonPlay );
    std::function<void(void* sender)> _buttonPlayCallback = std::bind(&StateMenu::ButtonPlayCB, this, std::placeholders::_1);
    m_ButtonPlay->SetCallBack( _buttonPlayCallback );
    m_ButtonOpt = std::shared_ptr < gui::Button > ( new gui::Button( m_SprMenuText ) );
    m_ButtonOpt->SetAnim(17);
    m_ButtonOpt->SetPos(140,300);
    m_ButtonOpt->SetWidthHeight(200,50);
    gui::GUIManager::GetInstance().AddElement( m_ButtonOpt );
    std::function<void(void* sender)> _buttonOptCallback = std::bind(&StateMenu::ButtonOptionCB, this, std::placeholders::_1);
    m_ButtonOpt->SetCallBack( _buttonOptCallback );
    m_ButtonExit = std::shared_ptr < gui::Button > ( new gui::Button( m_SprMenuText ) );
    m_ButtonExit->SetAnim(16);
    m_ButtonExit->SetPos(140,400);
    m_ButtonExit->SetWidthHeight(300,50);
    gui::GUIManager::GetInstance().AddElement( m_ButtonExit );
    std::function<void(void* sender)> _buttonExitCallback = std::bind(&StateMenu::ButtonExitCB, this, std::placeholders::_1);
    m_ButtonExit->SetCallBack( _buttonExitCallback );
    m_ButtonInfo = std::shared_ptr < gui::Button > ( new gui::Button( m_SprMenuText ) );
    m_ButtonInfo->SetAnim(24);
    m_ButtonInfo->SetPos(g_screen_w/2 - 50 - 25,500);
    m_ButtonInfo->SetWidthHeight(60,60);
    gui::GUIManager::GetInstance().AddElement( m_ButtonInfo );
    std::function<void(void* sender)> _buttonInfoCallback = std::bind(&StateMenu::ButtonInfoCB, this, std::placeholders::_1);
    m_ButtonInfo->SetCallBack( _buttonInfoCallback );
    m_ButtonSound = std::shared_ptr < gui::Button > ( new gui::Button( m_SprMenuText ) );
    m_ButtonSound->SetAnim(22);
    m_ButtonSound->SetPos(g_screen_w/2 + 50 - 25 ,500);
    m_ButtonSound->SetWidthHeight(60,60);
    gui::GUIManager::GetInstance().AddElement( m_ButtonSound );
    std::function<void(void* sender)> _buttonSoundCallback = std::bind(&StateMenu::ButtonSoundCB, this, std::placeholders::_1);
    m_ButtonSound->SetCallBack( _buttonSoundCallback );
    m_World = new PhysicTest ( m_Renderer );
    m_World->NewGame( m_SprMenuText );
    m_SubState = SUBSTATE::IGM;
}

int StateMenu::Update(const double delta_time) {
    switch( m_SubState ) {
    case SUBSTATE::IGM :
        m_AnimTextPlay->Update(delta_time);
        break;
    case SUBSTATE::MAIN :
        m_World->Update(delta_time);
        break;
    case SUBSTATE::INFO :
        //m_World->Render();
        break;
    }
    gui::GUIManager::GetInstance().Update(delta_time);
    return m_SubState;
}

void StateMenu::Render() {
    switch( m_SubState ) {
    case SUBSTATE::IGM :
        // m_AnimTextPlay->Render();
        for(unsigned int i=0; i<6; i++) {
            m_SprMenuText->PaintFrameNth( 58,0, 148 * i );
        }
        break;
    case SUBSTATE::MAIN :
        m_World->Render();
        break;
    case SUBSTATE::INFO :
        m_World->DrawInfo();
        break;
    }
    gui::GUIManager::GetInstance().Render();
}

void StateMenu::RenderDebug()	{
    m_World->RenderDebug();
}

void StateMenu::OnSuspending() {
}

void StateMenu::OnResuming(XDGameRenderer* renderer) {
    m_Renderer  = renderer;
    m_SprMenuText->SetSpriteBatch ( m_Renderer->SpriteBatchs[ XDGameRenderer::EBATCH::L0 ] );
    m_SprMenuText->LoadTexture( m_Renderer->GetDevice() );
    m_World->OnResuming ( m_SprMenuText.get() , m_Renderer );
}

void StateMenu::OnHandleInput(  const SINPUT& input ) {
    switch (input.pflag) {
    case FINPUT::P_MOVE :
        m_BackKeyOnIGM_Count=0;
        m_mousex = input.pposx;
        m_mousey = input.pposy;
        gui::GUIManager::GetInstance().CheckIntersect( input.pposx, input.pposy );
        m_World->PointerMoved( input.pposx, input.pposy );
        break;
    case FINPUT::P_PRESSED :
        m_BackKeyOnIGM_Count=0;
        gui::GUIManager::GetInstance().CheckIntersect( input.pposx, input.pposy );
        m_World->PointerPressed( input.pposx, input.pposy );
        break;
    case FINPUT::P_RELEASED :
        m_BackKeyOnIGM_Count=0;
        gui::GUIManager::GetInstance().Clicked( input.pposx, input.pposy );
        m_World->PointerReleased( input.pposx, input.pposy );
        break;
    case FINPUT::KEY_DOWN :
#define ESC_KEY 27
        if( input.pid == ESC_KEY ) {
            if( m_SubState == SUBSTATE::MAIN ) {
                m_BackKeyOnIGM_Count=0;
                m_ButtonPlay->SetVisible( 1 );
                m_ButtonOpt->SetVisible( 1 );
                m_ButtonExit->SetVisible( 1 );
                m_ButtonInfo->SetVisible( 1 );
                m_ButtonSound->SetVisible( 1 );
                m_World->OnHideIGM();
                m_SubState = SUBSTATE::IGM;
            } else 	if( m_SubState == SUBSTATE::INFO ) {
                m_BackKeyOnIGM_Count=0;
                m_ButtonPlay->SetVisible( 1 );
                m_ButtonOpt->SetVisible( 1 );
                m_ButtonExit->SetVisible( 1 );
                m_ButtonInfo->SetVisible( 1 );
                m_ButtonSound->SetVisible( 1 );
                m_SubState = SUBSTATE::IGM;
            } else 	if( m_SubState == SUBSTATE::IGM ) {
                m_BackKeyOnIGM_Count++;
                if( m_BackKeyOnIGM_Count > 1 ) {
                    m_SubState = SUBSTATE::STATEOUT;
                }
            }
        }
        break;
    default:
        break;
    }
}

StateMenu::~StateMenu() {
}

GameState::STATE StateMenu::Next() {
    return m_NextState;
}

void StateMenu::ButtonPlayCB(void* sender)	{
    // reset finish frame
    gui::GUIManager::GetInstance().CheckIntersect( -1,  -1 );
    m_ButtonPlay->SetVisible( 0 );
    m_ButtonOpt->SetVisible( 0 );
    m_ButtonExit->SetVisible( 0 );
    m_ButtonInfo->SetVisible( 0 );
    m_ButtonSound->SetVisible( 0 );
    m_SubState = SUBSTATE::MAIN;
    m_World->OnShowIGM();
}

void StateMenu::ButtonOptionCB(void* sender)	{
    // reset finish frame
    gui::GUIManager::GetInstance().CheckIntersect( -1,  -1 );
    m_ButtonPlay->SetVisible( 0 );
    m_ButtonOpt->SetVisible( 0 );
    m_ButtonExit->SetVisible( 0 );
    m_ButtonInfo->SetVisible( 0 );
    m_ButtonSound->SetVisible( 0 );
    m_World->NewGame( m_SprMenuText );
    m_SubState = SUBSTATE::MAIN;
}

void StateMenu::ButtonExitCB(void* sender)	{
    m_SubState = SUBSTATE::STATEOUT;
}

void StateMenu::ButtonInfoCB(void* sender)	{
    m_ButtonPlay->SetVisible( 0 );
    m_ButtonOpt->SetVisible( 0 );
    m_ButtonExit->SetVisible( 0 );
    m_ButtonInfo->SetVisible( 0 );
    m_ButtonSound->SetVisible( 0 );
    m_SubState = SUBSTATE::INFO;
}

void StateMenu::ButtonSoundCB(void* sender)	{
    m_Sound = !m_Sound;
    if(m_Sound) {
        m_ButtonSound->SetAnim(22);
    } else {
        m_ButtonSound->SetAnim(23);
    }
}

