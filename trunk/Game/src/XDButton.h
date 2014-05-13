/*
*  (c) 2013 XediXermawan < edi.ermawan@gmail.com >
*/
#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <functional>
#include "XDElementBase.h"

namespace gui {

class Button : public ElementBase {
  public:
    Button(const std::shared_ptr< XDSprite >& sharedptrSpr);
    Button();
    virtual ~Button();
    virtual bool IsIntersect(const int& posx,const int& posy);
    virtual void OnClicked(const int& posx, const int& posy);
    virtual void OnClickReleased(const int& posx, const int& posy);
    virtual void OnHover(const int& posx, const int& posy);
    virtual void Render();
    virtual void Update(const double delta_time);
    virtual int GetUID();
    virtual void SetPos(int vposx,int vposy);
    virtual void GetWidthHeight(int& gwidth, int& gheight );
    virtual void SetWidthHeight(int gwidth, int gheight );
    void SetCallBack(std::function< void(void* sender) >& function);
  private:
    std::function< void( void* sender ) > m_CallBackFunction;
};

};

#endif  // __BUTTON_H__