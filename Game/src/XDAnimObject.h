/*
*  (C) 2013 XediXermawan <edi.ermawan@gmail.com>
*/

#ifndef __ANIM_OBJECT_H__
#define __ANIM_OBJECT_H__

#include "XDSprite.h"

class XDAnimObject {
  public:
    enum ANIMLOOP {
        REPEATED =-1,
        ONCE   = 0
    };
    XDAnimObject(XDSprite* sprite,int posx=0,int posy=0);
    ~XDAnimObject();
    void Update(const double delta_time);
    void Render();
    void SetPos(int posx,int posy);
    void GetPos(int& posx,int& posy);
    void SetAnim(const int animindex);
    int GetAnim() const;
    void SetAnimLoop(ANIMLOOP type) {
        m_AnimLoop = type;
    };
    void ShowAfterFinish(bool show) {
        m_ShowAfterFinish = show;
    };
    void SetFinishFrame(int frame) {
        m_FinishFrame = frame;
    }
    void FlipHorizontally();
    void SetScale(float scale);
    bool IsLastFrame();
  private:
    int m_AnimIndex;
    int m_CurrentFrame;
    int m_AnimLoop;
    int m_PosX;
    int m_PosY;
    double m_DeltaTime;
    bool m_ShowAfterFinish;
    int m_FinishFrame;
    XDSprite* m_Sprite;
};

#endif // __ANIM_OBJECT_H__