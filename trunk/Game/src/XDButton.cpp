/*
*  (c) 2013 XediXermawan < edi.ermawan@gmail.com >
*/
#include "pch.h"
#include "XDButton.h"

namespace gui {

Button::Button(const std::shared_ptr< XDSprite >& sharedptrSpr) :ElementBase(sharedptrSpr)	{
    m_Anim->SetAnimLoop(XDAnimObject::ANIMLOOP::ONCE);
    m_Anim->ShowAfterFinish(true);
    m_FrameHover = 1;
}

Button::Button() : ElementBase(nullptr)	{
}

Button::~Button()	{
}

bool Button::IsIntersect(const int& posx, const int& posy)	{
    if( ( posx > m_PosX ) && ( posx < (m_PosX + m_Width) ) &&
            ( posy > m_PosY ) && ( posy < (m_PosY + m_Height)) ) {
        return true;
    }
    return false;
}

void Button::OnClicked(const int& posx, const int& posy)	{
    if( IsIntersect(posx,posy) ) {
        m_CallBackFunction(this);
    }
}

void Button::OnClickReleased(const int& posx, const int& posy)	{
}

void Button::OnHover(const int& posx, const int& posy)	{
    if( IsIntersect(posx,posy) ) {
        m_Anim->SetFinishFrame(m_FrameHover);
    } else {
        m_Anim->SetFinishFrame(0);
    }
}

void Button::Render()	{
    m_Anim->Render();
}

void Button::Update(const double delta_time)	{
    m_Anim->Update(delta_time);
}

int Button::GetUID()	{
    return 0; // todo
}

void Button::SetPos(int vposx,int vposy)	{
    m_PosX = vposx;
    m_PosY = vposy;
    m_Anim->SetPos(vposx,vposy);
}

void Button::GetWidthHeight(int& gwidth, int& gheight )	{
    gwidth  = m_Width;
    gheight = m_Height;
}

void Button::SetWidthHeight(int gwidth, int gheight )	{
    m_Width  = gwidth;
    m_Height = gheight;
}

void Button::SetCallBack(std::function< void(void* sender) >& function) {
    m_CallBackFunction = function;
}

}