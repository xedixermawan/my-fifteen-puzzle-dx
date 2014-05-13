/*
*  (c) 2013 XediXermawan < edi.ermawan@gmail.com >
*/
#include "pch.h"
#include "XDSpriteLoader.h"
#include <iostream>

// xml
void SpriteLoaderXML::Load(std::string& filename, int** * modules,int& vnmodule, std::vector< FrameDesc<int>* >& framedesc, std::vector< AnimDesc<int>* >& animdesc ) {
    TiXmlDocument doc(filename.c_str());
    bool loadOkay = doc.LoadFile();
    assert(loadOkay);
    if (loadOkay) {
        TiXmlHandle docHandle( &doc );
        // image path
        TiXmlElement* element = docHandle.FirstChild("sprites").FirstChild("image").FirstChild("path").Element();
        std::string image_path =element->GetText();
        // modules
        element = docHandle.FirstChild("sprites").FirstChild("modules").Element();
        std::string nStr_module = element->Attribute("nModules");
        int n_module =atoi(nStr_module.c_str());
        vnmodule = n_module;
        (*modules) = new int* [n_module];
        for(int i=0; i<n_module; i++) {
            (*modules)[i] = new int[SpriteConstant::N_MODULE_DEF];
        }
        element = docHandle.FirstChild("sprites").FirstChild("modules").FirstChild("nRow").Element();
        for(int i=0; i<n_module; i++) {
            std::string module_value = element->GetText();
            std::string temp("");
            char spaceChar =' ';
            unsigned int idx=0;
            for(unsigned int j=0; j<module_value.size(); j++) {
                if(module_value[j]==spaceChar ) {
                    (*modules)[i][idx++] = atoi( temp.c_str() );
                    temp ="";
                } else {
                    temp.push_back(module_value[j]);
                }
            }
            (*modules)[i][idx] = atoi( temp.c_str() );
            element = element->NextSiblingElement();
        }
        // frames
        element = docHandle.FirstChild("sprites").FirstChild("frames").Element();
        std::string nStr_frame = element->Attribute("nFrames");
        int n_frame = atoi(nStr_frame.c_str());
        framedesc.resize(n_frame);
        element = docHandle.FirstChild("sprites").FirstChild("frames").FirstChild("itemFrame").Element();
        for(int i=0; i<n_frame; i++) {
            std::string str_frame_id         = element->Attribute("ID");
            std::string str_n_moduleFrame    = element->Attribute("nModFrame");
            std::string str_frame_name       = element->Attribute("name");
            int frame_id = atoi( str_frame_id.c_str() );
            int n_moduleFrame = atoi (str_n_moduleFrame.c_str());
            framedesc[i] = new FrameDesc<int>(n_moduleFrame,frame_id);
            int n_moduleframe = atoi(str_n_moduleFrame.c_str());
            TiXmlElement* elementInside = element->FirstChildElement();
            for(int j=0; j<n_moduleframe; j++) {
                std::string frame_value = elementInside->GetText();
                std::string temp("");
                char spaceChar =' ';
                unsigned int idx=0;
                for(unsigned int k=0; k<frame_value.size(); k++) {
                    if(frame_value[k]==spaceChar ) {
                        framedesc[i]->m_Modules[j][idx++] = atoi( temp.c_str() );
                        temp ="";
                    } else {
                        temp.push_back(frame_value[k]);
                    }
                }
                framedesc[i]->m_Modules[j][idx++] = atoi( temp.c_str() );
                elementInside = elementInside->NextSiblingElement();
            }
            element = element->NextSiblingElement();
        }
        // animations
        element = docHandle.FirstChild("sprites").FirstChild("anims").Element();
        std::string str_n_anims = element->Attribute("nAnims");
        int n_anim =atoi(str_n_anims.c_str());
        animdesc.resize(n_anim);
        element = docHandle.FirstChild("sprites").FirstChild("anims").FirstChild("itemAnim").Element();
        for(int i=0; i<n_anim; i++) {
            std::string str_anim_id         = element->Attribute("ID");
            std::string str_n_frameAnim     = element->Attribute("nAnimFrame");
            std::string str_anim_name       = element->Attribute("name");
            int n_frameAnim = atoi(str_n_frameAnim.c_str());
            animdesc[i] = new AnimDesc<int>(n_frameAnim);
            int n_frameanim = atoi(str_n_frameAnim.c_str());
            TiXmlElement* elementInside = element->FirstChildElement();
            for(int j=0; j<n_frameanim; j++) {
                std::string anim_value = elementInside->GetText();
                std::string temp("");
                char spaceChar =' ';
                unsigned int idx=0;
                for(unsigned int k=0; k<anim_value.size(); k++) {
                    if(anim_value[k]==spaceChar ) {
                        animdesc[i]->m_Frames[j][idx++] = atoi( temp.c_str() );
                        temp ="";
                    } else {
                        temp.push_back(anim_value[k]);
                    }
                }
                animdesc[i]->m_Frames[j][idx++] = atoi( temp.c_str() );
                elementInside = elementInside->NextSiblingElement();
            }
            element = element->NextSiblingElement();
        }
    } else {
        printf("Failed ");
    }
}

//binary
void SpriteLoaderBIN::Load(std::string& filename,int** * modules,int& vnmodule, std::vector< FrameDesc<int>* >& framedesc, std::vector< AnimDesc<int>* >& animdescc ) {
    // TODO
    bool implemented = false;
    assert(implemented);
}