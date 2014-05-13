/*
*  (c) 2013 XediXermawan < edi.ermawan@gmail.com >
*/
#include "pch.h"
#include "XDAnimObject.h"

XDAnimObject::XDAnimObject(XDSprite* sprite,int posx,int posy)
    : m_AnimIndex(0),
      m_CurrentFrame(0),
      m_AnimLoop(ANIMLOOP::REPEATED),
      m_ShowAfterFinish(false),
      m_DeltaTime(0.0f),
      m_FinishFrame(0) {
    m_Sprite = sprite;
    m_PosX = posx;
    m_PosY = posy;
}

XDAnimObject::~XDAnimObject() {
}

void XDAnimObject::SetPos(int posx,int posy) {
    m_PosX = posx;
    m_PosY = posy;
}

void XDAnimObject::GetPos(int& posx,int& posy) {
    posx = m_PosX;
    posy = m_PosY;
}

void XDAnimObject::Render() {
    if(m_AnimLoop > 0 || m_AnimLoop == ANIMLOOP::REPEATED ) {
        m_Sprite->PaintFrameAnim(m_AnimIndex,m_CurrentFrame,m_PosX,m_PosY);
    } else if(m_ShowAfterFinish) {
        m_Sprite->PaintFrameAnim(m_AnimIndex, m_FinishFrame , m_PosX , m_PosY);
    }
}

void XDAnimObject::Update(const double delta_time) {
    if(m_AnimLoop > 0 || m_AnimLoop == ANIMLOOP::REPEATED ) {
        m_DeltaTime += delta_time ;
        if(m_DeltaTime > 0.1f) {
            m_DeltaTime = 0.0f;
            if(m_CurrentFrame < m_Sprite->GetNFrameAnim(m_AnimIndex) - 1 ) {
                m_CurrentFrame++;
            } else {
                m_CurrentFrame = 0;
                if(m_AnimLoop > 0 ) {
                    m_AnimLoop--;
                }
            }
        }
    }
}

void XDAnimObject::SetAnim(const int animindex)  {
    if(m_AnimIndex != animindex) {
        m_AnimIndex = animindex;
        m_CurrentFrame = 0;
    }
}

int XDAnimObject::GetAnim() const  {
    return m_AnimIndex;
}

void XDAnimObject::FlipHorizontally() {
    m_Sprite->FlipHorizontally();
}

void XDAnimObject::SetScale(float scale) {
    m_Sprite->SetScale(scale);
}

bool XDAnimObject::IsLastFrame()	{
    return (m_CurrentFrame == ( m_Sprite->GetNFrameAnim(m_AnimIndex) - 1) ) ;
}