/*
*  (c) 2013 XediXermawan < edi.ermawan@gmail.com >
*/
#ifndef __XDGAME_H__
#define __XDGAME_H__

#include <iostream>
#include <cassert>

#include "XDGameRenderer.h"
#include "XDGameState.h"
#include "XDGlobalConst.h"

struct VERTEX {
    float X, Y, Z;    // vertex position
};

class XDGame {
  public:
#include "XDProperties.hpp"
  public :
    XDGame();
    ~XDGame();
    void Initialize(HWND& hwindow);
#if WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
    void Initialize(ID3D11Device1* device, ID3D11DeviceContext1* context);
    void Update(const double delta_time, ID3D11Device1* device, _In_ ID3D11DeviceContext1* context,ID3D11RenderTargetView* renderTargetView);
#endif
    void Update(const double delta_time);
    void Render();
    void ShutDown();
    void OnSuspending();
    void OnResuming();
    void OnHandleInput( const SINPUT& input );
  private:
    std::shared_ptr< XDGameRenderer > m_Renderer;
    std::unique_ptr< GameState > m_CurrentState;
    std::unique_ptr<SpriteFont> m_TestFont;
};

#endif // __XDGAME_H__