/*
*  (c) 2013 XediXermawan < edi.ermawan@gmail.com >
*/
#include "pch.h"
#include "XDStateLoading.h"
#include "XDGameRenderer.h"
#include "XDGUIManager.h"

#include <iostream>	//test

StateLoading::StateLoading(XDGameRenderer* renderer) : GameState(renderer)   {
    m_NextState = STATE::MENU;
    m_StateStartTime = steady_clock::now();
}

void StateLoading::Initialize() {
    m_SprLoading  = std::shared_ptr< XDSprite> ( new XDSprite (std::string("xedisoftware.xml") ) ) ;
    m_AnimLoading = std::shared_ptr< XDAnimObject> (  new XDAnimObject( m_SprLoading.get() , 0 ,0) );
    m_SprLoading->SetSpriteBatch ( m_Renderer->SpriteBatchs[ XDGameRenderer::EBATCH::L0 ] );
    m_SprLoading->LoadTexture( m_Renderer->GetDevice() );
    m_SubState = SUBSTATE::MAIN;
    //gui::GUIManager::GetInstance().Load("menu_02.ep");
    //TiXmlDocument doc("menu_02.ep");
    //bool loadOkay = doc.LoadFile();
    //assert(loadOkay);
    //if (loadOkay) {
    //	TiXmlHandle docHandle( &doc );
    //	TiXmlNode * node = docHandle.FirstChild("Document").FirstChild("Properties").ToNode()->NextSibling();
    //
    //	TiXmlNode * page = node->FirstChild("Page");
    //	while( page!= nullptr )
    //	{
    //		// properties page
    //		TiXmlNode * page_props = page->FirstChild("Properties");
    //		TiXmlElement * page_prop_element = page_props->FirstChildElement("Property");
    //		std::string page_name  = page_prop_element->GetText();
    //		std::cout<< page_name.c_str() <<std::endl;
    //		std::cout<<"  properties"<<std::endl;
    //		while( page_prop_element!= nullptr )
    //		{
    //			std::string page_props_attr  = page_prop_element->Attribute("name");
    //			const char* cv  = page_prop_element->GetText();
    //			if( cv!=0)
    //				std::cout<<"    "<< page_props_attr.c_str() <<" : "<<cv<<std::endl;
    //			const char* cvl = page_prop_element->Value();
    //			if( cvl!=0)
    //				std::cout<<"    "<< page_props_attr.c_str() <<" : "<<cvl<<std::endl;
    //			page_prop_element = page_prop_element->NextSiblingElement();
    //		}
    //		// content
    //		TiXmlNode * page_content = page->FirstChild("Properties")->NextSibling();
    //		TiXmlNode * GrafElement = page_content->FirstChild("g");
    //		std::cout<<"  content"<<std::endl;
    //		while( GrafElement!=nullptr)
    //		{
    //			// graf attribute
    //			std::string gf_type = GrafElement->ToElement()->Attribute("p:type");
    //			std::string gf_def  = GrafElement->ToElement()->Attribute("p:def");
    //			std::string gf_id   = GrafElement->ToElement()->Attribute("id");
    //			std::string gf_tr   = GrafElement->ToElement()->Attribute("transform");
    //			std::cout<<"    "<< gf_type.c_str() <<std::endl;
    //			std::cout<<"    "<< gf_def.c_str() <<std::endl;
    //			std::cout<<"    "<< gf_id.c_str() <<std::endl;
    //			std::cout<<"    "<< gf_tr.c_str() <<std::endl;
    //			std::cout<<"    "<<"--"<<std::endl;
    //			// graf childs
    //			TiXmlNode * GrafElement_metadata = GrafElement->FirstChild("p:metadata")->FirstChild("p:property");
    //			while( GrafElement_metadata!=nullptr)
    //			{
    //				std::string gf_element_prop = GrafElement_metadata->ToElement()->Attribute("name");
    //				std::cout<<"     "<< gf_element_prop.c_str() <<" : "<<GrafElement_metadata->ToElement()->GetText()<<std::endl;
    //				GrafElement_metadata = GrafElement_metadata->NextSibling();
    //			}
    //			GrafElement = GrafElement->NextSibling();
    //		}
    //		//next page
    //		page = page->NextSibling();
    //	}
    //}
}

int StateLoading::Update(const double delta_time) {
    m_AnimLoading->Update(delta_time);
    steady_clock::time_point _time_now = steady_clock::now();
    double _delta_time_seconds = ( duration_cast<milliseconds>(_time_now - m_StateStartTime).count() ) ;
    if( 2000 < _delta_time_seconds) {
        m_SubState = SUBSTATE::STATEOUT;
    }
    // gui::GUIManager::GetInstance().Update(delta_time);
    return m_SubState;
}

void StateLoading::Render() {
    switch (m_SubState) {
    case GameState::INIT:
        break;
    case GameState::MAIN:
        m_AnimLoading->Render();
        break;
    case GameState::DEINIT:
        break;
    case GameState::STATEOUT:
        break;
    default:
        break;
    }
    gui::GUIManager::GetInstance().Render();
}

void StateLoading::RenderDebug()	{
}

void StateLoading::OnSuspending() {
}

void StateLoading::OnResuming(XDGameRenderer* renderer) {
    m_Renderer  = renderer;
    m_SprLoading  = std::shared_ptr< XDSprite> ( new XDSprite (std::string("xedisoftware.xml") ) ) ;
    m_AnimLoading = std::shared_ptr< XDAnimObject> (  new XDAnimObject( m_SprLoading.get() , 0 ,0) );
    m_SprLoading->SetSpriteBatch ( m_Renderer->SpriteBatchs[ XDGameRenderer::EBATCH::L0 ] );
    m_SprLoading->LoadTexture( m_Renderer->GetDevice() );
}

void StateLoading::OnHandleInput(  const SINPUT& input ) {
}

StateLoading::~StateLoading() {
}

GameState::STATE StateLoading::Next()	{
    return m_NextState;
}
