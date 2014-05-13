/*
*  (c) 2013 XediXermawan < edi.ermawan@gmail.com >
*/
#ifndef _PhysicTest_H_
#define _PhysicTest_H_

#include <Box2d\Box2d.h>
#include "XDGameRenderer.h"
#include <chrono>

using std::chrono::duration_cast;
using std::chrono::seconds;
using std::chrono::steady_clock;

class QueryCallback : public b2QueryCallback {
  public:
    QueryCallback(const b2Vec2& point) {
        m_point = point;
        m_fixture = NULL;
    }

    bool ReportFixture(b2Fixture* fixture) {
        b2Body* body = fixture->GetBody();
        if (body->GetType() == b2_dynamicBody) {
            bool inside = fixture->TestPoint(m_point);
            if (inside) {
                m_fixture = fixture;
                // We are done, terminate the query.
                return false;
            }
        }
        // Continue the query.
        return true;
    }

    b2Vec2 m_point;
    b2Fixture* m_fixture;
};


struct FiveTeenDef {
    unsigned int screen_width;
    unsigned int screen_height;
    unsigned int row_count;
    unsigned int col_count;
    float box_width;
    float box_hight;
    float start_point_x;
    float start_point_y;
};

class SquareData {
  public:
    SquareData(XDAnimObject* anim) {
        m_Animation = anim;
    }
    ~SquareData() {
        delete m_Animation;
    }
    XDAnimObject* GetAnim() {
        return m_Animation;
    }
    unsigned int Number;
    unsigned int IsOnPlace;
    unsigned int IsClicked;
  private:
    XDAnimObject* m_Animation;
};

class PhysicTest {
  public:
    PhysicTest(XDGameRenderer* renderer);
    PhysicTest(XDGameRenderer* renderer,const FiveTeenDef& def);
    ~PhysicTest();
    bool IsInRect(const int& s_px,const int& s_py,const int& s_w,const int& s_h,const int& px,const int& py);
    void NewGame(std::shared_ptr< XDSprite>& sprite);
    void Update(const double delta_time);
    void Render();
    void RenderDebug();
    void SetSpriteCell();
    void PointerPressed(const int& px,const int& py);
    void PointerReleased(const int& px,const int& py);
    void PointerMoved(const int& px,const int& py);
    void GenerateNumbers();
    void OnHideIGM();
    void OnShowIGM();
    void OnResuming(XDSprite* sprite,XDGameRenderer* renderer);
    void DrawInfo();
  private:
    XDGameRenderer* m_Renderer;
    std::shared_ptr< XDSprite >  m_SprButtons;
    std::shared_ptr< XDSprite >  m_SprMainBg;
    std::shared_ptr< XDAnimObject >  m_animBG;
    std::shared_ptr< XDAnimObject >  m_AnimSolved;
    std::shared_ptr<SpriteFont> m_SprFont;
    steady_clock::time_point m_StartTime;
    double m_DurationTime;
    wchar_t m_DurationTimeStr[256];
    bool m_IsSolved;
    //physics
    std::unique_ptr< b2World > m_world;
    std::vector<b2Body*> m_bodyVec;
    b2Body* m_simgroundbody;
    b2MouseJoint* m_mouseJoint;
    b2Vec2 m_mouseWorld;
    b2Vec2 m_startPoint,m_widthHeight;

    float32 m_timeStep;
    int32 m_velocityIterations;
    int32 m_positionIterations;
    float32 m_box_width;
    float32 m_box_hight;
    int32 m_screen_height;
    int32 m_screen_width;
    uint32 m_row_count;
    uint32 m_col_count;
    unsigned int m_numbers[15];

    b2Vec2 ScreenToWorld(b2Vec2& screen);
    b2Vec2 WorldToScreen(b2Vec2& world);
    void CreateBound(const b2Vec2& startPoint,b2Vec2& widthHeight);
    b2Body* CreateBox(const b2Vec2& pos,b2Vec2& widthHeight);
    void FormatTime(const double timesecond);
};

#endif // _PhysicTest_H_