/*
*  (c) 2013 XediXermawan < edi.ermawan@gmail.com >
*/
#include "pch.h"
#include "XDGameRenderer.h"

using namespace Microsoft::WRL;

#if WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
XDGameRenderer::XDGameRenderer(ID3D11Device1* device, ID3D11DeviceContext1* context,unsigned int screenWidth,unsigned int screenHeight) {
    m_ScreenWidth = screenWidth;
    m_ScreenHeight = screenHeight;
    m_TestInputLayout=0;
    SetDeviceContext(device,context);
    Init();
}
#endif

XDGameRenderer::XDGameRenderer(HWND& hwindow, unsigned int screenWidth,unsigned int screenHeight) {
    m_HWindow = &hwindow;
    m_ScreenWidth = screenWidth;
    m_ScreenHeight = screenHeight;
    m_TestInputLayout=0;
    Init();
}

XDGameRenderer::~XDGameRenderer() {
}

void XDGameRenderer::Init() {
#if WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP
    D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_9_1 }; //  //* D3D_FEATURE_LEVEL_10_0 */ /*D3D_FEATURE_LEVEL_11_0  D3D_FEATURE_LEVEL_10_0*/
    // create swapchain & device
    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    swapChainDesc.BufferDesc.Width  = (UINT) m_ScreenWidth;
    swapChainDesc.BufferDesc.Height = (UINT) m_ScreenHeight;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    // swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_CENTERED;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 2;
    swapChainDesc.OutputWindow = (*m_HWindow);
    swapChainDesc.Windowed = TRUE;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapChainDesc.Flags = 0;
    HRESULT hr = D3D11CreateDeviceAndSwapChain(
                     NULL,
                     D3D_DRIVER_TYPE_HARDWARE, // original : D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP
                     NULL,
                     D3D11_CREATE_DEVICE_SINGLETHREADED,
                     featureLevels,
                     _countof(featureLevels),
                     D3D11_SDK_VERSION,
                     &swapChainDesc,
                     &m_SwapChain,
                     &m_Device,
                     NULL,
                     &m_DevContext);
    assert(SUCCEEDED(hr));
    // Get swap chain's back buffer, create its render target view and set that view as render target
    ID3D11Texture2D* backbuffer;
    hr = m_SwapChain->GetBuffer(0, __uuidof(*backbuffer), (void**)&backbuffer);
    assert(SUCCEEDED(hr));
    hr = m_Device->CreateRenderTargetView(backbuffer, NULL, &m_RenderTarget);
    assert(SUCCEEDED(hr));
    m_DevContext->OMSetRenderTargets(1, &m_RenderTarget, NULL);
    // Set viewport
    D3D11_VIEWPORT viewport;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width  = (FLOAT) m_ScreenWidth;
    viewport.Height = (FLOAT) m_ScreenHeight;
    viewport.MinDepth = 0.f;
    viewport.MaxDepth = 1.f;
    m_DevContext->RSSetViewports(1, &viewport);
#endif
    //Init2D();
    stateObjects = new CommonStates ( m_Device );
}

void XDGameRenderer::Init2D() {
    //----- raster state setting--------//
    D3D11_RASTERIZER_DESC rasterizerState;
    rasterizerState.CullMode = D3D11_CULL_NONE;
    rasterizerState.FillMode = D3D11_FILL_SOLID;
    rasterizerState.FrontCounterClockwise = FALSE;
    rasterizerState.DepthBias = false;
    rasterizerState.DepthBiasClamp = 0;
    rasterizerState.SlopeScaledDepthBias = 0;
    rasterizerState.DepthClipEnable = true;
    rasterizerState.ScissorEnable = false;
    rasterizerState.MultisampleEnable = false;
    rasterizerState.AntialiasedLineEnable = true;
    ComPtr<ID3D11RasterizerState> pRS;
    m_Device->CreateRasterizerState( &rasterizerState, &pRS );
    m_DevContext->RSSetState(pRS.Get());
    //----- depth stencil setting--------//
    D3D11_DEPTH_STENCIL_DESC dsDesc;
    // Depth test parameters
    dsDesc.DepthEnable = false;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL; // default is D3D11_COMPARISON_LESS
    // Stencil test parameters
    dsDesc.StencilEnable = true;
    dsDesc.StencilReadMask = 0xFF;
    dsDesc.StencilWriteMask = 0xFF;
    // Stencil operations if pixel is front-facing
    dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    // Stencil operations if pixel is back-facing
    dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    // Create depth stencil state
    ComPtr<ID3D11DepthStencilState> pDSState;
    m_Device->CreateDepthStencilState(&dsDesc, &pDSState);
    m_DevContext->OMSetDepthStencilState(pDSState.Get(), 1);
    // blend setting
    D3D11_BLEND_DESC bDesc;
    ZeroMemory(&bDesc, sizeof(D3D11_BLEND_DESC));
    // Create an alpha enabled blend state description.
    bDesc.RenderTarget[0].BlendEnable = TRUE;
    bDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    bDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    bDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    bDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    bDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    bDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    bDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    ComPtr<ID3D11BlendState> pBState;
    m_Device->CreateBlendState(&bDesc, &pBState);
    float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    UINT sampleMask   = 0xffffffff;
    m_DevContext->OMSetBlendState(pBState.Get(), blendFactor, sampleMask);
}

void XDGameRenderer::Update(const double delta_time) {
}

#if WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
void XDGameRenderer::Update(_In_ ID3D11Device1* device, _In_ ID3D11DeviceContext1* context, _In_ ID3D11RenderTargetView* renderTargetView) {
    SetDeviceContext( device, context );
    SetRenderTarget( renderTargetView );
    ComPtr<ID3D11Resource> renderTargetViewResource;
    renderTargetView->GetResource(&renderTargetViewResource);
    ComPtr<ID3D11Texture2D> backBuffer;
    renderTargetViewResource.As(&backBuffer);
    // Cache the rendertarget dimensions in our helper class for convenient use.
    D3D11_TEXTURE2D_DESC backBufferDesc;
    backBuffer->GetDesc(&backBufferDesc);
    m_renderTargetSize_X  = static_cast<float>(backBufferDesc.Width);
    m_renderTargetSize_Y = static_cast<float>(backBufferDesc.Height);
    // Set the rendering viewport to target the entire window.
    CD3D11_VIEWPORT viewport(
        0.0f,
        0.0f,
        m_renderTargetSize_X,
        m_renderTargetSize_Y
    );
    context->RSSetViewports(1, &viewport);
    //set 2D state
    Init2D();
}
#endif

void XDGameRenderer::InitResources() {
    m_Test_spriteBatch = new DirectX::SpriteBatch(m_DevContext);
    for(int i=0; i< (int) EBATCH::MAX ; i++) {
        SpriteBatchs[i] = new DirectX::SpriteBatch(m_DevContext);
    }
    m_BasicEffect     = new BasicEffect(m_Device);
    m_TestBasicEffect = new BasicEffect(m_Device);
    m_PrimitiveBatch = new PrimitiveBatch<VertexPositionColor>(m_DevContext);
    m_TestPrimitiveBatch = new PrimitiveBatch<VertexPositionColorTexture>(m_DevContext);
    //testPrite = new XDSprite(std::string("luck.xml")); // "walkanim.xml"
    //testPrite->SetSpriteBatch(m_Test_spriteBatch);
    //testPrite->LoadTexture(m_Device);
//	CreateDDSTextureFromFile(m_Device, L"backgmain.DDS", nullptr, &m_TestTexture, MAXSIZE_T);
    //testAnim = new XDAnimObject(testPrite,20,300);
}

void XDGameRenderer::ClearScene() const {
    // Clear background with bg_color
    //const float bg_color[] =  {0.0f, 0.0f, 0.0f, 0.8f}; //  {0.0f, 0.2f, 0.4f, 1.0f}; // {0.8f, 0.6f, 1.0f, 1.0f};
    //m_DevContext->ClearRenderTargetView(
    //    m_RenderTarget,
    //    bg_color
    //    );
    // clear
    m_DevContext->OMSetRenderTargets(
        1,
        &m_RenderTarget,
        NULL
    );
    const float crazyPink[] = {0.0f, 0.0f, 0.0f, 1.0f };
    m_DevContext->ClearRenderTargetView(
        m_RenderTarget,
        crazyPink
    );
    //m_DevContext->ClearDepthStencilView(
    //    m_depthStencilView.Get(),
    //    D3D11_CLEAR_DEPTH,
    //    1.0f,
    //    0
    //    );
}

void XDGameRenderer::Present() const {
    // switch the back buffer and the front buffer
    m_SwapChain->Present(1, 0);
}

void XDGameRenderer::UpdateTestMode(const double delta_time) {
    //testAnim->Update(delta_time);
}

void XDGameRenderer::CustomShaderSetter()	{
    DirectX::XMMATRIX projection =XMMatrixOrthographicOffCenterRH(0, 480, 800, 0, 0, 1);
    m_TestBasicEffect->SetProjection( projection );
    m_TestBasicEffect->SetVertexColorEnabled(true);
    m_TestBasicEffect->SetTextureEnabled(true);
    m_TestBasicEffect->SetTexture( m_TestTexture );
    void const* shaderByteCode;
    size_t byteCodeLength;
    m_TestBasicEffect->GetVertexShaderBytecode( &shaderByteCode, &byteCodeLength );
    HRESULT hr = m_Device->CreateInputLayout(   VertexPositionColorTexture::InputElements,
                 VertexPositionColorTexture::InputElementCount,
                 shaderByteCode, byteCodeLength,
                 &m_TestInputLayout
                                            );
    m_DevContext->IASetInputLayout( m_TestInputLayout );
    m_TestBasicEffect->Apply( m_DevContext );
}

void XDGameRenderer::RenderTestMode() {
    ClearScene();
#if 0
    std::function< void() > shadersetcb = std::bind( &XDGameRenderer::CustomShaderSetter, this );
    DirectX::XMVECTORF32 acx = { 0.0f, 1.0f, 0.0f };
    m_Test_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred,nullptr,nullptr,nullptr,nullptr, shadersetcb );
    RECT rect1;
    rect1.top    = 10;
    rect1.left   = 10;
    rect1.bottom = 100 + 400;
    rect1.right  = 100 + 700;
    m_Test_spriteBatch->Draw(m_TestTexture, XMFLOAT2(0, 0), &rect1, Colors::White, 0.0f, XMFLOAT2(0.0f, 0.0f), XMFLOAT2( 1.0f, 1.0f ) , DirectX::SpriteEffects_None, 0.0f);
    m_Test_spriteBatch->End();
#endif
    auto blendState = stateObjects->AlphaBlend();
    auto depthStencilState = stateObjects->DepthNone();
    auto rasterizerState = stateObjects->CullCounterClockwise();
    auto samplerState = stateObjects->LinearClamp();
    m_DevContext->OMSetBlendState(blendState, nullptr, 0xFFFFFFFF);
    m_DevContext->OMSetDepthStencilState(depthStencilState, 0);
    m_DevContext->RSSetState(rasterizerState);
    m_DevContext->PSSetSamplers(0, 1, &samplerState);
    CustomShaderSetter();
#if 0
    m_PrimitiveBatch->Begin();
    VertexPositionColor v1;
    v1.position.x =0;
    v1.position.y =0;
    v1.position.z=0;
    v1.color.x =122;
    v1.color.y =122;
    v1.color.z =0;
    VertexPositionColor v2;
    v2.position.x =0;
    v2.position.y =300;
    v2.position.z=0;
    v2.color.x =3;
    v2.color.y =122;
    v2.color.z =0;
    VertexPositionColor v3;
    v3.position.x =300;
    v3.position.y =300;
    v3.position.z=0;
    v3.color.x =3;
    v3.color.y =122;
    v3.color.z =0;
    m_PrimitiveBatch->DrawTriangle( v1, v2 , v3);
    m_PrimitiveBatch->End();
#endif
    m_TestPrimitiveBatch->Begin();
    VertexPositionColorTexture v1;
    v1.position.x =0;
    v1.position.y =0;
    v1.position.z=0;
    v1.color.x =1.0f;
    v1.color.y =1.0f;
    v1.color.z =1.0f;
    v1.color.w =1.0f;
    v1.textureCoordinate= DirectX::XMFLOAT2(0.0f, 0.0f);
    VertexPositionColorTexture v2;
    v2.position.x =0;
    v2.position.y =300;
    v2.position.z=0;
    v2.color.x =1.0f;
    v2.color.y =1.0f;
    v2.color.z =1.0f;
    v2.color.w =1.0f;
    v2.textureCoordinate= DirectX::XMFLOAT2(1.0f, 0.0f);
    VertexPositionColorTexture v3;
    v3.position.x =300;
    v3.position.y =300;
    v3.position.z=0;
    v3.color.x =1.0f;
    v3.color.y =1.0f;
    v3.color.z =1.0f;
    v3.color.w = 1.0f;
    v3.textureCoordinate= DirectX::XMFLOAT2(1.0f, 1.0f);
    m_TestPrimitiveBatch->DrawTriangle( v1, v3 , v2);
    m_TestPrimitiveBatch->End();
    Present();
}

void XDGameRenderer::DrawLine(const XMFLOAT2& point1,const XMFLOAT2& point2) {
#if 1
    /*
    XMMATRIX mymatrix =  DirectX::XMMatrixIdentity() * XMMatrixRotationAxis(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), XM_PIDIV2);
    mymatrix = XMMatrixTranspose(mymatrix);
    m_Test_spriteBatch->Begin();
    RECT rect1;
    rect1.top    = 0;
    rect1.left   = 0;
    rect1.bottom = 100;
    rect1.right  = 100;
    m_Test_spriteBatch->Draw(m_TestTexture, XMFLOAT2(100, 100), &rect1, Colors::Red, 0.0f, XMFLOAT2(0.0f, 0.0f), XMFLOAT2( 1.0f, 1.0f ) , DirectX::SpriteEffects_None, 0.0f);
    m_Test_spriteBatch->End();
    */
    auto blendState = stateObjects->AlphaBlend();
    auto depthStencilState = stateObjects->DepthNone();
    auto rasterizerState = stateObjects->CullCounterClockwise();
    auto samplerState = stateObjects->LinearClamp();
    m_DevContext->OMSetBlendState(blendState, nullptr, 0xFFFFFFFF);
    m_DevContext->OMSetDepthStencilState(depthStencilState, 0);
    m_DevContext->RSSetState(rasterizerState);
    m_DevContext->PSSetSamplers(0, 1, &samplerState);
    DirectX::XMMATRIX projection =XMMatrixOrthographicOffCenterRH(0, 800, 480, 0, 0, 1);
    m_BasicEffect->SetProjection( projection );
    m_BasicEffect->SetVertexColorEnabled(true);
    void const* shaderByteCode;
    size_t byteCodeLength;
    m_BasicEffect->GetVertexShaderBytecode( &shaderByteCode, &byteCodeLength );
    ComPtr<ID3D11InputLayout>  m_TestInputLayout2;
    //std::shared_ptr<ID3D11InputLayout>  m_TestInputLayout2;
    ///ID3D11InputLayout* ris ;
    HRESULT hr = m_Device->CreateInputLayout(   VertexPositionColorTexture::InputElements,
                 VertexPositionColorTexture::InputElementCount,
                 shaderByteCode, byteCodeLength,
                 &m_TestInputLayout2
                                            );
    m_DevContext->IASetInputLayout( m_TestInputLayout2.Get() );
    m_BasicEffect->Apply( m_DevContext );
    m_PrimitiveBatch->Begin();
    VertexPositionColor v1;
    v1.position.x =point1.x;
    v1.position.y =point1.y;
    v1.position.z =0.0f;
    v1.color.x    =1.0f;
    v1.color.y    =1.0f;;
    v1.color.z    =1.0f;;
    VertexPositionColor v2;
    v2.position.x =point2.x;
    v2.position.y =point2.y;
    v2.position.z =0.0f;
    v2.color.x    =1.0f;;
    v2.color.y    =1.0f;;
    v2.color.z    =1.0f;;
    m_PrimitiveBatch->DrawLine( v1, v2);
    m_PrimitiveBatch->End();
#endif
}

void XDGameRenderer::DrawRect(const XMFLOAT2& pos,const XMFLOAT2& widthheight, bool drawcl) {
#if 1
    auto blendState = stateObjects->AlphaBlend();
    auto depthStencilState = stateObjects->DepthNone();
    auto rasterizerState = stateObjects->CullCounterClockwise();
    auto samplerState = stateObjects->LinearClamp();
    m_DevContext->OMSetBlendState(blendState, nullptr, 0xFFFFFFFF);
    m_DevContext->OMSetDepthStencilState(depthStencilState, 0);
    m_DevContext->RSSetState(rasterizerState);
    m_DevContext->PSSetSamplers(0, 1, &samplerState);
    DirectX::XMMATRIX projection =XMMatrixOrthographicOffCenterRH(0, 480, 800, 0, 0, 1);
    m_BasicEffect->SetProjection( projection );
    m_BasicEffect->SetVertexColorEnabled(true);
    void const* shaderByteCode;
    size_t byteCodeLength;
    m_BasicEffect->GetVertexShaderBytecode( &shaderByteCode, &byteCodeLength );
    ComPtr<ID3D11InputLayout>  m_TestInputLayout2;
    //ID3D11InputLayout* ris ;
    HRESULT hr = m_Device->CreateInputLayout(   VertexPositionColorTexture::InputElements,
                 VertexPositionColorTexture::InputElementCount,
                 shaderByteCode, byteCodeLength,
                 &m_TestInputLayout2
                                            );
    m_DevContext->IASetInputLayout( m_TestInputLayout2.Get() );
    m_BasicEffect->Apply( m_DevContext );
    m_PrimitiveBatch->Begin();
    // top
    VertexPositionColor v1;
    v1.position.x =pos.x;
    v1.position.y =pos.y;
    v1.position.z =0.0f;
    v1.color.x    =1.0f;
    v1.color.y    =1.0f;;
    v1.color.z    =1.0f;;
    VertexPositionColor v2;
    v2.position.x =pos.x + widthheight.x;
    v2.position.y =pos.y;
    v2.position.z =0.0f;
    v2.color.x    =1.0f;
    v2.color.y    =1.0f;
    v2.color.z    =1.0f;
    m_PrimitiveBatch->DrawLine( v1, v2);
    //bottom
    v1.position.x =pos.x;
    v1.position.y =pos.y + widthheight.y;
    v1.position.z =0.0f;
    v1.color.x    =1.0f;
    v1.color.y    =1.0f;;
    v1.color.z    =1.0f;;
    v2.position.x =pos.x + widthheight.x;
    v2.position.y =pos.y + widthheight.y;
    v2.position.z =0.0f;
    v2.color.x    =1.0f;
    v2.color.y    =1.0f;
    v2.color.z    =1.0f;
    m_PrimitiveBatch->DrawLine( v1, v2);
    // left
    v1.position.x =pos.x;
    v1.position.y =pos.y;
    v1.position.z =0.0f;
    v1.color.x    =1.0f;
    v1.color.y    =1.0f;;
    v1.color.z    =1.0f;;
    v2.position.x =pos.x ;
    v2.position.y =pos.y + widthheight.y;
    v2.position.z =0.0f;
    v2.color.x    =1.0f;
    v2.color.y    =1.0f;
    v2.color.z    =1.0f;
    m_PrimitiveBatch->DrawLine( v1, v2);
    //right
    v1.position.x =pos.x + widthheight.x;
    v1.position.y =pos.y ;
    v1.position.z =0.0f;
    v1.color.x    =1.0f;
    v1.color.y    =1.0f;;
    v1.color.z    =1.0f;;
    v2.position.x =pos.x + widthheight.x;
    v2.position.y =pos.y + widthheight.y;
    v2.position.z =0.0f;
    v2.color.x    =1.0f;
    v2.color.y    =1.0f;
    v2.color.z    =1.0f;
    m_PrimitiveBatch->DrawLine( v1, v2);
    if( drawcl) {
        v1.position.x =pos.x;
        v1.position.y =pos.y;
        v1.position.z =0.0f;
        v1.color.x    =1.0f;
        v1.color.y    =1.0f;;
        v1.color.z    =1.0f;;
        v2.position.x =pos.x + widthheight.x;
        v2.position.y =pos.y + widthheight.y;
        v2.position.z =0.0f;
        v2.color.x    =1.0f;
        v2.color.y    =1.0f;
        v2.color.z    =1.0f;
        m_PrimitiveBatch->DrawLine( v1, v2);
    }
    m_PrimitiveBatch->End();
#endif
}

void XDGameRenderer::Begin() {
    XMMATRIX mymatrix =  DirectX::XMMatrixTranslation(0.0f,0.0f,0.0f) *  DirectX::XMMatrixScaling(1.0f,1.0f,1.0f) * XMMatrixRotationAxis(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), XM_PIDIV2);
    //mymatrix = XMMatrixTranspose(mymatrix);
    //(DirectX::SpriteSortMode_Deferred,nullptr,nullptr,nullptr,nullptr,nullptr, mymatrix );
    SpriteBatchs[ XDGameRenderer::EBATCH::L0 ]->Begin();
    SpriteBatchs[ XDGameRenderer::EBATCH::L1 ]->Begin();
    SpriteBatchs[ XDGameRenderer::EBATCH::L2 ]->Begin();
}

void XDGameRenderer::End() {
    SpriteBatchs[ XDGameRenderer::EBATCH::L0 ]->End();
    SpriteBatchs[ XDGameRenderer::EBATCH::L1 ]->End();
    SpriteBatchs[ XDGameRenderer::EBATCH::L2 ]->End();
}