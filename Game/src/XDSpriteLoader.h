/*
*  (C) 2013 XediXermawan <edi.ermawan@gmail.com>
*/
#ifndef __SPRITE_LOADER_H__
#define __SPRITE_LOADER_H__

#include <string>
#include <vector>
#include "XDFrameAnimDescription.h"
#include <tinyxml.h>

/*
* base class
*/
class SpriteLoader {
  public:
    virtual void Load(std::string& filename, int** * modules,int& vnmodule, std::vector< FrameDesc<int>* >& framedesc, std::vector< AnimDesc<int>* >& animdesc ) = 0;
};

/*
* XML loader
*/
class SpriteLoaderXML : public SpriteLoader {
  public:
    void Load(std::string& filename, int** * modules,int& vnmodule, std::vector< FrameDesc<int>* >& framedesc, std::vector< AnimDesc<int>* >& animdesc );

};

/*
* binary loader
*/
class SpriteLoaderBIN : public SpriteLoader {
  public:
    void Load(std::string& filename, int** * modules,int& vnmodule, std::vector< FrameDesc<int>* >& framedesc, std::vector< AnimDesc<int>* >& animdesc);
};

#endif  // __SPRITE_LOADER_H__