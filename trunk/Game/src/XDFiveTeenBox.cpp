/*
*  (c) 2013 XediXermawan < edi.ermawan@gmail.com >
*/
#include "pch.h"
#include "XDFiveTeenBox.h"
#include "XDSoundSystem.h"
#include "XDFuncUtils.h"
#include "XDFileSystem.h"
#include <iostream>
#include <algorithm>

#undef min

// #define DRAW_DEBUG_LINE

#define PIXEL_PER_METER 30.0f
#define OFFSETBOX (6)

extern float g_screen_w;
extern float g_screen_h;

PhysicTest::PhysicTest(XDGameRenderer* renderer)	{
    m_Renderer = renderer;
    m_screen_width  = g_screen_w;
    m_screen_height = g_screen_h;
    m_row_count     = 4;
    m_col_count     = 4;
    m_box_width     = 114.0f;
    m_box_hight     = 114.0f;
    m_startPoint    = b2Vec2(-2+ (480-456)/2,-12+((800-456)/2));
    m_widthHeight   = b2Vec2( (m_col_count * m_box_width) + OFFSETBOX, (m_row_count * m_box_hight) + OFFSETBOX);
}

PhysicTest::PhysicTest(XDGameRenderer* renderer, const FiveTeenDef& def)   {
    m_Renderer = renderer;
    m_screen_width  = def.screen_width;
    m_screen_height = def.screen_height;
    m_row_count     = def.row_count;
    m_col_count     = def.col_count;
    m_box_width     = def.box_width;
    m_box_hight     = def.box_hight;
    m_startPoint    = b2Vec2( def.start_point_x, def.start_point_y );
}

void PhysicTest::CreateBound(const b2Vec2& startPoint,b2Vec2& widthHeight)	{
    b2BodyDef bodyDefground;
    bodyDefground.type = b2_staticBody;
    m_simgroundbody    = m_world->CreateBody(&bodyDefground);
    b2EdgeShape simBound;
    simBound.Set( startPoint, b2Vec2( startPoint.x+widthHeight.x, startPoint.y ) );                                              // bottom
    m_simgroundbody->CreateFixture(&simBound,0.0f);
    simBound.Set(b2Vec2(startPoint.x,startPoint.y+widthHeight.y),b2Vec2(startPoint.x+widthHeight.x,startPoint.y+widthHeight.y)); // top
    m_simgroundbody->CreateFixture(&simBound,0.0f);
    simBound.Set( startPoint,b2Vec2(startPoint.x,startPoint.y+widthHeight.y) );                                                  // left
    m_simgroundbody->CreateFixture(&simBound,0.0f);
    simBound.Set(b2Vec2(startPoint.x+widthHeight.x,startPoint.y),b2Vec2(startPoint.x+widthHeight.x,startPoint.y+widthHeight.y)); // right
    m_simgroundbody->CreateFixture(&simBound,0.0f);
}

b2Body* PhysicTest::CreateBox(const b2Vec2& pos,b2Vec2& widthHeight)	{
    // Define the dynamic body. We set its position and call the body factory.
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    float32 tw= (m_box_width/2.0f) / PIXEL_PER_METER;
    float32 th= (m_box_hight/2.0f) / PIXEL_PER_METER;
    bodyDef.position.Set( pos.x + tw, pos.y + th );
    b2Body* bodyPtr = m_world->CreateBody(&bodyDef);
    // Define another box shape for our dynamic body.
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox( tw , th  );
    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    // Set the box density to be non-zero, so it will be dynamic.
    fixtureDef.density = 10.0f;
    //fixtureDef.restitution = 0.4f;
    // Override the default friction.
    fixtureDef.friction = 0.1f;
    // Add the shape to the body.
    bodyPtr->CreateFixture(&fixtureDef);
    bodyPtr->SetFixedRotation(true);
    return bodyPtr;
}

void PhysicTest::NewGame(std::shared_ptr< XDSprite>& sprite)	  {
#if WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
    int idxsound =  RandomRange(0,3);
    if( !XDSoundSystem::GetInstance().IsSoundPlaying( idxsound ) ) {
        XDSoundSystem::GetInstance().PlaySound( idxsound );
    }
#endif
    // destroy old data
    for(std::vector<b2Body*>::iterator it = m_bodyVec.begin(); it!=m_bodyVec.end(); it++) {
        SquareData* data= static_cast<SquareData*> ((*it)->GetUserData() );
        (*it)->SetUserData( nullptr );
        m_world->DestroyBody( (*it) );
        delete data;
    }
    m_bodyVec.clear();
    //reset timer
    m_DurationTime = 0;
    m_StartTime    = steady_clock::now();
    m_IsSolved     = false;
    m_SprButtons = sprite;
    m_SprButtons->SetSpriteBatch ( m_Renderer->SpriteBatchs[ XDGameRenderer::EBATCH::L1 ] );
    m_SprButtons->LoadTexture( m_Renderer->GetDevice() );
    m_AnimSolved = std::shared_ptr< XDAnimObject>(new XDAnimObject(sprite.get(), 18, 287));
    m_AnimSolved->SetAnim(26);
    m_AnimSolved->SetFinishFrame( 5 );
    m_AnimSolved->ShowAfterFinish( true );
    m_AnimSolved->SetAnimLoop( XDAnimObject::ANIMLOOP::ONCE );
    m_SprMainBg = std::shared_ptr< XDSprite>(new XDSprite(std::string("backgmain.xml")));
    m_SprMainBg->SetSpriteBatch ( m_Renderer->SpriteBatchs[ XDGameRenderer::EBATCH::L0 ] );
    m_SprMainBg->LoadTexture( m_Renderer->GetDevice() );
    m_animBG = std::shared_ptr< XDAnimObject>(new XDAnimObject(m_SprMainBg.get(), 0, 0));
    m_animBG->SetAnim(0);
    m_animBG->SetPos(0,0);
    std::string corrected_path="";
    FileSystem::Get_ROFolder(corrected_path, "armalite.spritefont" );
    assert ( ( FileSystem::FileExist ( corrected_path.c_str() ) == true ) );
    m_SprFont = std::shared_ptr < SpriteFont >(new SpriteFont(m_Renderer->GetDevice(), UTF8ToWChar(corrected_path.c_str())));
    m_timeStep           = 1.0f / 60.0f;
    m_velocityIterations = 40;
    m_positionIterations = 4;
    b2Vec2 gravity;
    gravity.Set(0.0f, 0.0f);
    m_world = std::unique_ptr< b2World> ( new b2World(gravity) );
    CreateBound( b2Vec2(m_startPoint.x / PIXEL_PER_METER,m_startPoint.y / PIXEL_PER_METER),
                 b2Vec2(m_widthHeight.x / PIXEL_PER_METER, m_widthHeight.y / PIXEL_PER_METER) );
    // get solvable 15 numbers arrangements
    GenerateNumbers();
    int bc = 0;
    int bc_total = (m_col_count*m_row_count)-1;
    for(unsigned int i=0; i<m_row_count; i++) {
        for(unsigned int j=0; j<m_col_count; j++) {
            if( bc < bc_total )  {
                XDAnimObject* _anim_obj_temp = new XDAnimObject( m_SprButtons.get() , 0, 0 ) ;
                _anim_obj_temp->SetAnim( m_numbers[bc]-1 );
                b2Vec2 px = b2Vec2(m_startPoint.x+(j)*m_box_width,m_startPoint.y+(3*m_box_hight)-(i)*m_box_hight);
                b2Vec2 py = b2Vec2(m_box_width,m_box_hight);
                px.x = px.x / PIXEL_PER_METER;
                px.y = px.y / PIXEL_PER_METER;
                py.x = py.x / PIXEL_PER_METER;
                py.y = py.y / PIXEL_PER_METER;
                b2Body* body = CreateBox(px,py);
                SquareData*   data = new SquareData( _anim_obj_temp );
                data->Number    = m_numbers[bc] ;
                data->IsOnPlace = 0;
                data->IsClicked =0;
                body->SetUserData( (void*) data );
                m_bodyVec.push_back( body );
            }
            bc++;
        }
    }
    m_mouseJoint = NULL;
}

void PhysicTest::SetSpriteCell()	{
}

PhysicTest::~PhysicTest()	{
}

bool PhysicTest::IsInRect(const int& s_px,const int& s_py,const int& s_w,const int& s_h,const int& px,const int& py) {
    bool IsInRect = false;
    if( px > s_px && px < s_px+s_w && py > s_py && py < s_py+s_h )	{
        IsInRect = true;
    }
    return IsInRect;
}

void PhysicTest::Update(const double delta_time)	{
    m_world->Step(m_timeStep, m_velocityIterations, m_positionIterations);
    if( m_IsSolved )	{
        m_AnimSolved->Update( delta_time );
    } else  {
        m_animBG->Update(delta_time);
        m_IsSolved = true;
        // test win
        for(std::vector<b2Body*>::iterator it = m_bodyVec.begin(); it!=m_bodyVec.end(); it++) {
            b2Vec2 position = (*it)->GetPosition();
            position.x = position.x  * PIXEL_PER_METER;
            position.y = position.y  * PIXEL_PER_METER;
            XMFLOAT2 _pos( position.x - m_box_width /2.0f ,(m_screen_height - position.y)- m_box_hight/2.0f );
            XMFLOAT2 _wh( m_box_width,m_box_hight);
            //m_Renderer->DrawRect(_pos,_wh);
            SquareData* data= static_cast<SquareData*> ((*it)->GetUserData() );
            unsigned int _row= (data->Number-1) / 4 ;
            unsigned int _col= (data->Number-1) % 4 ;
            data->IsOnPlace = 0;
            if( IsInRect( m_startPoint.x +_col*m_box_width ,  m_startPoint.y + _row*m_box_hight, m_box_width,m_box_hight,_pos.x+50,_pos.y+50) )  {
                data->IsOnPlace = 1;
            } else  {
                m_IsSolved = false;
            }
        }
        double durationAdd = m_DurationTime + ( duration_cast<seconds>( steady_clock::now()  - m_StartTime).count() ) ;
        FormatTime( durationAdd );
    }
    if (m_mouseJoint) {
        m_mouseJoint->SetTarget(m_mouseWorld);
    }
}

void PhysicTest::Render()	{
    // draw BG
#ifndef DRAW_DEBUG_LINE
    m_animBG->Render();
#endif
    // draw bound
    b2Vec2 sec = WorldToScreen( b2Vec2(m_startPoint.x ,m_startPoint.y ) );
    XMFLOAT2 _pos_bound( m_startPoint.x, m_screen_height - m_startPoint.y-m_widthHeight.y );
    XMFLOAT2 _wh_bound( (m_widthHeight.x),(m_widthHeight.y));
    m_Renderer->DrawRect(_pos_bound,_wh_bound);
    // draw box
    for(std::vector<b2Body*>::iterator it = m_bodyVec.begin(); it!=m_bodyVec.end(); it++) {
        b2Vec2 position = (*it)->GetPosition();
        position.x = position.x  * PIXEL_PER_METER;
        position.y = position.y  * PIXEL_PER_METER;
        XMFLOAT2 _pos( position.x - m_box_width /2.0f ,(m_screen_height - position.y)- m_box_hight/2.0f );
        XMFLOAT2 _wh( m_box_width,m_box_hight);
        SquareData* data= static_cast<SquareData*> ( (*it)->GetUserData() );
        data->GetAnim()->SetPos( _pos.x , _pos.y );
#ifndef DRAW_DEBUG_LINE
        if( data->IsClicked ) {
            m_SprButtons->PaintFrameAnim( data->Number-1, 0, _pos.x , _pos.y, Colors::LightSteelBlue  );
        } else {
            data->GetAnim()->Render();
        }
#else
        m_Renderer->DrawRect(_pos,_wh,true);
#endif
    }
    m_SprFont->DrawString( m_Renderer->SpriteBatchs[ XDGameRenderer::EBATCH::L1 ] , m_DurationTimeStr , XMFLOAT2(370, 150), Colors::DarkGray );
    if( m_IsSolved )  {
        m_AnimSolved->Render();
    }
}

void PhysicTest::FormatTime(const double timesecond) {
    unsigned int min_ = timesecond / 60;
    unsigned int sec_ = timesecond - min_*60;
    unsigned int hour_ = min_ / 60;
    min_ = min_ - hour_*60;
    wchar_t tempSec[4];
    if( sec_ < 10 )
        swprintf_s( tempSec, L"0%i",sec_);
    else
        swprintf_s( tempSec, L"%i",sec_);
    wchar_t tempMin[4];
    if( min_ < 10 )
        swprintf_s( tempMin, L"0%i",min_);
    else
        swprintf_s( tempMin, L"%i",min_);
    swprintf( m_DurationTimeStr, L"0%i:%s:%s", hour_, tempMin, tempSec );
}

void PhysicTest::RenderDebug()	{
}

void PhysicTest::PointerPressed(const int& px,const int& py)	{
    if( m_IsSolved ) {
        m_IsSolved=false;
        NewGame( m_SprButtons );
        return;
    }
    m_mouseWorld = ScreenToWorld(b2Vec2( (float) px,(float) py));
    if( m_mouseJoint != NULL )
        return;
    b2AABB aabb;
    b2Vec2 d;
    d.Set(0.001f, 0.001f);
    aabb.lowerBound = m_mouseWorld - d;
    aabb.upperBound = m_mouseWorld + d;
    // Query the world for overlapping shapes.
    QueryCallback callback(m_mouseWorld);
    m_world->QueryAABB(&callback, aabb);
    if (callback.m_fixture) {
        b2Body* body = callback.m_fixture->GetBody();
        b2MouseJointDef md;
        md.bodyA = m_simgroundbody;
        md.bodyB = body;
        md.target =m_mouseWorld;
        md.maxForce = 5000.0f * body->GetMass();
        md.collideConnected = true;
        md.frequencyHz = 60;
        m_mouseJoint = (b2MouseJoint*)m_world->CreateJoint(&md);
        body->SetAwake(true);
        if (m_mouseJoint) {
            m_mouseJoint->SetTarget(m_mouseWorld);
        }
        SquareData* data= static_cast<SquareData*> ( body->GetUserData() );
        data->IsClicked = 1;
    }
}

void PhysicTest::PointerReleased(const int& px,const int& py)	{
    if (m_mouseJoint) {
        m_world->DestroyJoint(m_mouseJoint);
        m_mouseJoint = NULL;
        for(std::vector<b2Body*>::iterator it = m_bodyVec.begin(); it!=m_bodyVec.end(); it++) {
            SquareData* data= static_cast<SquareData*> ( (*it)->GetUserData() );
            if( data->IsClicked ) {
                data->IsClicked = 0;
            }
        }
    }
}

void PhysicTest::PointerMoved(const int& px,const int& py)	{
    m_mouseWorld =  ScreenToWorld(b2Vec2( (float)px,(float)py));
}

b2Vec2 PhysicTest::ScreenToWorld(b2Vec2& screen)  {
    return b2Vec2(screen.x / PIXEL_PER_METER, (m_screen_height - screen.y) / PIXEL_PER_METER );
}

b2Vec2 PhysicTest::WorldToScreen(b2Vec2& world)   {
    return b2Vec2(world.x * PIXEL_PER_METER,  m_screen_height - (world.y * PIXEL_PER_METER) );
}

void PhysicTest::GenerateNumbers()   {
    for(int i=0; i<15; i++) {
        m_numbers[i]=i+1;
    }
    bool issolvable=false;
    while( !issolvable ) {
        for(int i=0; i<15; i++) {
            unsigned int nrandomer=0;
            for(int j=0; j<1000; j++) {
                nrandomer  = RandomRange(0,14);
                unsigned int nrandomer2 = RandomRange(0,14);
                unsigned int temp       = m_numbers[nrandomer];
                m_numbers[nrandomer]      = m_numbers[nrandomer2];
                m_numbers[nrandomer2]     = temp;
            }
            unsigned int temp       = m_numbers[i];
            m_numbers[i]      = m_numbers[nrandomer];
            m_numbers[nrandomer]     = temp;
        }
        //test solvable : http://mathworld.wolfram.com/15Puzzle.html
        unsigned int N_ni=0;
        for(int i=0; i<15; i++) {
            for(int j=i+1; j<15; j++) {
                if( m_numbers[i] > m_numbers[j] ) {
                    N_ni++;
                }
            }
        }
        if( N_ni % 2 == 0)
            issolvable=true;
    }
}

void PhysicTest::OnHideIGM() {
    m_DurationTime = m_DurationTime + ( duration_cast<seconds>( steady_clock::now()  - m_StartTime).count() ) ;
}

void PhysicTest::OnShowIGM() {
    m_StartTime = steady_clock::now();
}

void PhysicTest::OnResuming(XDSprite* sprite,XDGameRenderer* renderer) {
    m_Renderer = renderer;
    m_SprButtons  = std::unique_ptr< XDSprite> ( sprite ) ;
    m_SprButtons->SetSpriteBatch ( m_Renderer->SpriteBatchs[ XDGameRenderer::EBATCH::L1 ] );
    m_SprButtons->LoadTexture( m_Renderer->GetDevice() );
    m_SprMainBg->SetSpriteBatch ( m_Renderer->SpriteBatchs[ XDGameRenderer::EBATCH::L0 ] );
    m_SprMainBg->LoadTexture( m_Renderer->GetDevice() );
    std::string corrected_path="";
    FileSystem::Get_ROFolder(corrected_path, "armalite.spritefont" );
    assert ( ( FileSystem::FileExist ( corrected_path.c_str() ) == true ) );
    m_SprFont = std::unique_ptr < SpriteFont >(new SpriteFont( m_Renderer->GetDevice(), UTF8ToWChar (corrected_path.c_str() ) ));
}

void PhysicTest::DrawInfo() {
#define T1 L"Nth Prime Software"
#define T2 L"Copyright - 2014"
#define T3 L"Programmer, Artist, Tester"
#define T4 L"Edi Ermawan"
#define INFO_X_OFFSET 125
#define INFO_Y_OFFSET 280
    m_SprFont->DrawString( m_Renderer->SpriteBatchs[ XDGameRenderer::EBATCH::L1 ] , T1 , XMFLOAT2( INFO_X_OFFSET   , INFO_Y_OFFSET   ), Colors::DarkGray );
    m_SprFont->DrawString( m_Renderer->SpriteBatchs[ XDGameRenderer::EBATCH::L1 ] , T2 , XMFLOAT2( INFO_X_OFFSET+20, INFO_Y_OFFSET+30), Colors::DarkGray );
    // m_SprFont->DrawString( m_Renderer->SpriteBatchs[ XDGameRenderer::EBATCH::L1 ] , T3 , XMFLOAT2( INFO_X_OFFSET   , INFO_Y_OFFSET+60), Colors::DarkGray );
    // m_SprFont->DrawString( m_Renderer->SpriteBatchs[ XDGameRenderer::EBATCH::L1 ] , T4 , XMFLOAT2( INFO_X_OFFSET   , INFO_Y_OFFSET+90), Colors::DarkGray );
}