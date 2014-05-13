/*
*  (c) 2013 XediXermawan < edi.ermawan@gmail.com >
*/
#include "pch.h"
#include "XDSprite.h"
#include "XDFileSystem.h"
#include "XDFuncUtils.h"

XDSprite::XDSprite(const std::string& filename)
    : m_FlipH(1)  ,
      m_Scale(1.0f) {
    m_SpriteName = filename;
    m_Loader = new SpriteLoaderXML();
    std::string corrected_path="";
    FileSystem::Get_ROFolder(corrected_path, filename);
    m_Loader->Load(corrected_path,&m_Modules,m_Nmodule, m_FrameDesc,m_AnimDesc );
#ifndef NDEBUG
    // test module
    for(int i=0; i< m_Nmodule ; i++) {
        for(int n=0; n<SpriteConstant::N_MODULE_DEF; n++) {
            int vann = m_Modules[i][n];
        }
    }
    // test frames
    for(unsigned int i=0; i<m_FrameDesc.size(); i++) {
        unsigned int n = m_FrameDesc[i]->m_ModuleCount;
        for(unsigned int j=0; j<n; j++) {
            for(unsigned int k=0; k<SpriteConstant::N_FRAME_DEF; k++) {
                unsigned int vann = m_FrameDesc[i]->m_Modules[j][k];
            }
        }
    }
    // test anims
    for(unsigned int i=0; i<m_AnimDesc.size(); i++) {
        unsigned int n = m_AnimDesc[i]->m_FrameCount;
        for(unsigned int j=0; j<n; j++) {
            for(unsigned int k=0; k<SpriteConstant::N_ANIM_DEF; k++) {
                unsigned int vann = m_AnimDesc[i]->m_Frames[j][k];
            }
        }
    }
#endif
}

void XDSprite::SetSpriteBatch(SpriteBatch* spritebatch) {
    m_SpriteBatch = spritebatch;
}

void XDSprite::LoadTexture(ID3D11Device* dxdevice) {
    std::string corrected_path="";
    char temp[64];
    GetFilenameNoExt(temp,m_SpriteName.c_str());
    strcat_s(temp,".dds");
    FileSystem::Get_ROFolder(corrected_path, temp );
    assert ( ( FileSystem::FileExist ( corrected_path.c_str() ) == true ) );
    CreateDDSTextureFromFile(dxdevice, UTF8ToWChar (corrected_path.c_str() ), nullptr, &m_ShaderResView, MAXSIZE_T);
    assert( m_ShaderResView );
}

void XDSprite::SetSRV( ID3D11ShaderResourceView* srv) {
    m_ShaderResView = srv;
}

int XDSprite::GetModuleIndexByID(int moduid) const {
    for(int i=0; i< m_Nmodule ; i++) {
        if(moduid == m_Modules[i][1]) {
            return i;
        }
    }
    return -1;
}

int XDSprite::GetFrameIndexByID(int frameuid) const {
    for(unsigned int i=0; i<m_FrameDesc.size(); i++) {
        if(frameuid == m_FrameDesc[i]->m_FrameID ) {
            return i;
        }
    }
    return -1;
}

void XDSprite::PaintModuleNth(unsigned int index,int offsetx,int offsety,FXMVECTOR color) {
    int _module_x = m_Modules[index][2];
    int _module_y = m_Modules[index][3];
    int _module_w = m_Modules[index][4];
    int _module_h = m_Modules[index][5];
    RECT rect1;
    rect1.top    = _module_y;
    rect1.left   = _module_x ;
    rect1.bottom = _module_y + _module_h;
    rect1.right  = _module_x +_module_w;
    DirectX::SpriteEffects flipsprite = SpriteEffects_None;
    if( m_FlipH == -1)
        flipsprite = SpriteEffects_FlipHorizontally;
    m_SpriteBatch->Draw( m_ShaderResView,
                         XMFLOAT2( (float) offsetx , (float) offsety),
                         &rect1,
                         color,
                         0.0f,
                         XMFLOAT2(0.0f, 0.0f),
                         XMFLOAT2( m_Scale, m_Scale ) ,
                         flipsprite,
                         0.0f);
}

void XDSprite::PaintFrameNth(unsigned int index, int offsetx, int offsety,FXMVECTOR color) {
    int frame_n = index;
    int _module_count = m_FrameDesc[frame_n]->m_ModuleCount;
    for(int i=0; i<_module_count; i++) {
        int _module_index = GetModuleIndexByID( m_FrameDesc[frame_n]->m_Modules[i][0] ) ;
        int _ox =  m_FrameDesc[frame_n]->m_Modules[i][1];
        int _oy =  m_FrameDesc[frame_n]->m_Modules[i][2];
        PaintModuleNth(_module_index, _ox + offsetx , _oy + offsety ,color);
    }
}

void XDSprite::PaintFrameAnim(unsigned int animIndex, unsigned int frameIndex, int offsetx, int offsety,FXMVECTOR color) {
    int _frame_uid  =  m_AnimDesc[animIndex]->m_Frames[frameIndex][0];
    int _frame_ox   =  m_AnimDesc[animIndex]->m_Frames[frameIndex][1];
    int _frame_oy   =  m_AnimDesc[animIndex]->m_Frames[frameIndex][2];
    int _frame_time =  m_AnimDesc[animIndex]->m_Frames[frameIndex][3];
    int _frame_index = GetFrameIndexByID( _frame_uid );
    PaintFrameNth(_frame_index, m_FlipH *_frame_ox * m_Scale + offsetx, _frame_oy * m_Scale + offsety , color);
}

int XDSprite::GetNFrameAnim(int animIndex) {
    return  m_AnimDesc[animIndex]->m_FrameCount;
}

XDSprite::~XDSprite() {
}

void XDSprite::FlipHorizontally()	{
    m_FlipH = m_FlipH * (-1);
}

void XDSprite::SetScale(float scale)	{
    m_Scale = scale;
}