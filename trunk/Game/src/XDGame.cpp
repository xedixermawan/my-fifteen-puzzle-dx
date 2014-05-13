/*
*  (c) 2013 XediXermawan < edi.ermawan@gmail.com >
*/
#include "pch.h"
#include <ctime>

#include "XDGame.h"
#include "XDFileSystem.h"
#include "XDCreateState.h"
#include "XDFuncUtils.h"

extern double g_fps_measure;
extern float g_screen_w;
extern float g_screen_h;
#if WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP
extern int g_exit_app;
#endif

//#define TEST_MODE

XDGame::XDGame()	{
    m_CurrentState = nullptr;
}

XDGame::~XDGame()	{
}
#if WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
void XDGame::Initialize(ID3D11Device1* device, ID3D11DeviceContext1* context)	 {
    m_Renderer = std::shared_ptr< XDGameRenderer > ( new XDGameRenderer( device,
                 context,
                 ( unsigned int ) Property::Windows::Width,
                 ( unsigned int ) Property::Windows::Height
                                                                       ) );
    m_Renderer->InitResources();
    if( m_CurrentState == nullptr ) {
        extern std::string g_rw_local_folder;
        extern std::string g_ro_local_folder;
        FileSystem::RWFolder.append(g_rw_local_folder);
        FileSystem::ROFolder.append(g_ro_local_folder);
        m_CurrentState = std::unique_ptr < GameState > ( CreateState::NewState ( GameState::STATE::LOADING , m_Renderer.get() ) );
        m_CurrentState->Initialize();
    }
    srand( time (NULL) );
}
#elif WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP
void XDGame::Initialize(HWND& hwindow) {
    FileSystem::RWFolder.append(".\\data");
    FileSystem::ROFolder.append(".\\data");
    m_Renderer = std::shared_ptr< XDGameRenderer > ( new XDGameRenderer( hwindow,
                 ( unsigned int ) g_screen_w,
                 ( unsigned int ) g_screen_h
                                                                       ) );
    m_Renderer->InitResources();
    m_CurrentState = std::unique_ptr < GameState > ( CreateState::NewState ( GameState::STATE::LOADING , m_Renderer.get() ) );
    m_CurrentState->Initialize();
    std::string corrected_path="";
    FileSystem::Get_ROFolder(corrected_path, "armalite.spritefont" );
    assert ( ( FileSystem::FileExist ( corrected_path.c_str() ) == true ) );
    m_TestFont = std::unique_ptr < SpriteFont >(new SpriteFont( m_Renderer->GetDevice(), UTF8ToWChar (corrected_path.c_str() ) ));
    srand( time (NULL) );
}
#endif

#if WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
void XDGame::Update(const double delta_time, ID3D11Device1* device, _In_ ID3D11DeviceContext1* context, ID3D11RenderTargetView* renderTargetView) {
    m_Renderer->Update(device,context,renderTargetView);
    Update(delta_time);
    Render();
}
#endif

void XDGame::Update(const double delta_time) {
#ifndef TEST_MODE
    GameState::SUBSTATE ret = ( GameState::SUBSTATE ) m_CurrentState->Update(delta_time);
    if( GameState::SUBSTATE::STATEOUT == ret ) {
        GameState::STATE newstate = m_CurrentState->Next();
        if( newstate == GameState::STATE::EXIT ) {
#if WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP
            //clean something here
            g_exit_app = 1;
#else
            // exit w/ crashing will be fast :p
            int* ptr=0;
            ptr[0]=0;
#endif
        } else {
            m_CurrentState = std::unique_ptr < GameState > ( CreateState::NewState ( newstate ,  m_Renderer.get() ) );
            m_CurrentState->Initialize();
        }
    }
    m_Renderer->Update(delta_time);
    //ParticleSystem::Instance()->Update( delta_time );
#else
    m_Renderer->UpdateTestMode(delta_time);
#endif
}

void XDGame::Render() {
#ifndef TEST_MODE
    m_Renderer->ClearScene();
    m_Renderer->Begin();
    m_CurrentState->Render();
    //	char cg_fps_measure[32];
    //sprintf(cg_fps_measure,"fps: %2.f",g_fps_measure);
    //wchar_t*  fp = UTF8ToWChar(cg_fps_measure);
    //m_TestFont->DrawString( m_Renderer->SpriteBatchs[ XDGameRenderer::EBATCH::L1 ] , fp , XMFLOAT2(10, 10), Colors::Red );
    //delete  fp;
    ////ParticleSystem::Instance()->Render();
    m_Renderer->End();
    m_CurrentState->RenderDebug();
#if WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP
    m_Renderer->Present();
#endif
#else
    m_Renderer->RenderTestMode();
#endif
}

void XDGame::ShutDown()	{
    // ParticleSystem::Destroy();
}

void XDGame::OnSuspending()	{
    m_CurrentState->OnSuspending();
}

void XDGame::OnResuming()	{
    m_CurrentState->OnResuming( m_Renderer.get() );
}

void XDGame::OnHandleInput( const SINPUT& input )	{
    m_CurrentState->OnHandleInput( input );
}