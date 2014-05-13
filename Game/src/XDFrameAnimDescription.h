/*
*  (C) 2013 XediXermawan <edi.ermawan@gmail.com>
*/
#ifndef __FRAMEANIM_DESC_H__
#define __FRAMEANIM_DESC_H__

#include "XDSpriteConstant.h"

/*
 *  class for hold a frame
 *  a frame is consist of modules
 */
template< class TypeData >
class FrameDesc {
  public:
    TypeData m_ModuleCount;
    TypeData m_FrameID;
    TypeData** m_Modules;

    FrameDesc(TypeData nmodules,TypeData frameid) {
        m_FrameID = frameid;
        m_ModuleCount = nmodules;
        m_Modules = new TypeData*[nmodules];
        for(int i=0; i<nmodules; i++) {
            m_Modules[i] = new TypeData[SpriteConstant::N_FRAME_DEF];
            for(int j=0; j<SpriteConstant::N_FRAME_DEF; j++) {
                m_Modules[i][j] = 0 ;
            }
        }
    }
    ~FrameDesc() {
        for(int i=0; i<m_ModuleCount; i++) {
            delete[] m_Modules[i];
        }
        delete[] m_Modules;
    }
  private:
    // copying is not allowed
    FrameDesc(FrameDesc const&);
    FrameDesc& operator= (FrameDesc const&);
};

/*
 *  class for hold an animation
 *  an animation is consist of frames
 */
template < class TypeData >
class AnimDesc {
  public:
    TypeData m_FrameCount;
    TypeData** m_Frames;
    AnimDesc(TypeData nframe) {
        m_FrameCount = nframe;
        m_Frames = new TypeData*[nframe];
        for(int i=0; i<nframe; i++) {
            m_Frames[i] = new TypeData[SpriteConstant::N_ANIM_DEF];
            for(int j=0; j<SpriteConstant::N_ANIM_DEF; j++) {
                m_Frames[i][j] = 0 ;
            }
        }
    }
    ~AnimDesc() {
        for(int i=0; i<m_FrameCount; i++) {
            delete[] m_Frames[i];
        }
        delete[] m_Frames;
    }
  private:
    // copying is not allowed
    AnimDesc(AnimDesc const&);
    AnimDesc& operator= (AnimDesc const&);
};

#endif // __FRAMEANIM_DESC_H__