/*
*  (c) 2013 XediXermawan < edi.ermawan@gmail.com >
*/
#ifndef __GLOBAL_CONST_H__
#define __GLOBAL_CONST_H__

enum FINPUT {
    P_MOVE,
    P_DOWN,
    P_PRESSED,
    P_RELEASED,
    B_DOWN,
    B_UP,
    KEY_DOWN,
    KEY_UP
};

enum KEYCODE {
    KeyALeft=37,
    KeyAUp=38,
    KeyARight=39,
    KeyADown=40,
    SPACE=32,
    CTRL=17
};

struct SINPUT {
    int pposx;
    int pposy;
    FINPUT pflag;
    int pid;
    int pcode;
    long ptimestamp;
};

#endif // __GLOBAL_CONST_H__