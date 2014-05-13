/*
*  (C) 2013 XediXermawan <edi.ermawan@gmail.com>
*/
#ifndef __SPRITECONSTANT_H__
#define __SPRITECONSTANT_H__

namespace SpriteConstant {
const int N_MODULE_DEF = 6;     // number of field in single module

const int N_FRAME_DEF  = 5; 	// number of field in single frame

const int N_ANIM_DEF   = 5; 	// number of field in single animation

const int BYTES_START_MODULE = 0xEFFE0EEF;  // unique bytes ( 4 byte ) signed starting module data

const int BYTES_START_FRAME  = 0xFFFA0EFF;  // unique bytes ( 4 byte ) signed starting frame data

const int BYTES_START_ANIM   = 0xEEEA0EFF;  // unique bytes ( 4 byte ) signed starting anim data
}

#endif // __SPRITECONSTANT_H__