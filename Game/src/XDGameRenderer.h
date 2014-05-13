/*
*  (C) 2013 XediXermawan <edi.ermawan@gmail.com>
*/
#ifndef __XDRENDERER_H__
#define __XDRENDERER_H__

#include "Effects.h"
#include "SpriteBatch.h"
#include "PrimitiveBatch.h"
#include "VertexTypes.h"
#include "SpriteFont.h"
#include "DDSTextureLoader.h"
#include "SimpleMath.h"
#include "CommonStates.h"

#if WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
#include <d3d11_1.h>
#endif

//test only:
#include "XDSprite.h"
#include "XDAnimObject.h"

using namespace std;
using namespace DirectX;

class XDGameRenderer {
  public:
    enum EBATCH {
        L0,
        L1,
        L2,
        L3,
        MAX
    };
#if WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
    XDGameRenderer(ID3D11Device1* device, ID3D11DeviceContext1* context,unsigned int screenWidth,unsigned int screenHeight);
#endif
    XDGameRenderer(HWND& hwindow, unsigned int screenWidth,unsigned int screenHeight);
    ~XDGameRenderer();
    void Update(const double delta_time);
#if WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
    void Update(_In_ ID3D11Device1* device, _In_ ID3D11DeviceContext1* context, _In_ ID3D11RenderTargetView* renderTargetView);
#endif
    void InitResources();
    void ClearScene() const;
    void Present() const;

    void UpdateTestMode(const double delta_time);
    void RenderTestMode();

    void CustomShaderSetter();

    void DrawLine(const XMFLOAT2& point1,const XMFLOAT2& point2);
    void DrawRect(const XMFLOAT2& pos,const XMFLOAT2& widthheight,bool drawcl=false);
    void Begin();
    void End();
#if WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
    void SetDeviceContext(ID3D11Device1* device, ID3D11DeviceContext1* context) {
        m_Device     = device ;
        m_DevContext = context;
    }
#endif
    void SetRenderTarget(ID3D11RenderTargetView* renderTargetView) {
        m_RenderTarget = renderTargetView;
    }

    ID3D11DeviceContext* GetDeviceContext()  {
        return m_DevContext;
    };
#if WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP
    ID3D11Device* GetDevice() const {
        return m_Device;
    };
#elif WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
    ID3D11Device1* GetDevice() const {
        return m_Device;
    };
#endif
    SpriteBatch* SpriteBatchs[EBATCH::MAX];

  private:
    unsigned int m_ScreenWidth;
    unsigned int m_ScreenHeight;
    HWND* m_HWindow;
#if WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP
    ID3D11Device* m_Device;
#elif WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
    ID3D11Device1* m_Device;
#endif
    ID3D11DeviceContext* m_DevContext;
    IDXGISwapChain* m_SwapChain;
    ID3D11RenderTargetView* m_RenderTarget;
    ID3D11Buffer* m_VertexBuffer;
    ID3D11Buffer* m_IndexBuffer;
    ID3D11VertexShader* m_VertexShader;
    ID3D11PixelShader* m_PixelShader;
    ID3D11InputLayout* m_InputLayout;
    ID3D11DepthStencilView*  m_depthStencilView;
    SpriteBatch* m_Test_spriteBatch;
    ID3D11ShaderResourceView* m_TestTexture;

    ID3D11InputLayout*  m_TestInputLayout;

    XMMATRIX                            m_World;
    XMMATRIX                            m_View;
    XMMATRIX                            m_Projection;

    BasicEffect* m_BasicEffect;
    PrimitiveBatch<VertexPositionColor>* m_PrimitiveBatch;

    BasicEffect* m_TestBasicEffect;
    PrimitiveBatch<VertexPositionColorTexture>* m_TestPrimitiveBatch;
    CommonStates* stateObjects;

    int    m_renderTargetSize_X;
    int    m_renderTargetSize_Y;

    XDSprite* testPrite;
    XDAnimObject* testAnim;
    void Init();
    void Init2D();
};

#endif // __XDRENDERER_H__