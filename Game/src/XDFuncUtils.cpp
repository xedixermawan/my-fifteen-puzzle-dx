/*
*  (c) 2013 XediXermawan < edi.ermawan@gmail.com >
*/
#include "pch.h"
#include "XDFuncUtils.h"
#include <cstdlib>


wchar_t* UTF8ToWChar(const char* str) {
    long b=0,
         c=0;
    if ((unsigned char)str[0]==0xEF && (unsigned char)str[1]==0xBB && (unsigned char)str[2]==0xBF)
        str+=3;
    for (const char* a=str; *a; a++)
        if (((unsigned char)*a)<128 || (*a&192)==192)
            c++;
    wchar_t* res=new wchar_t[c+1];
    res[c]=0;
    for (unsigned char* a=(unsigned char*)str; *a; a++) {
        if (!(*a&128))
            //Byte represents an ASCII character. Direct copy will do.
            res[b]=*a;
        else if ((*a&192)==128)
            //Byte is the middle of an encoded character. Ignore.
            continue;
        else if ((*a&224)==192)
            //Byte represents the start of an encoded character in the range
            //U+0080 to U+07FF
            res[b]=((*a&31)<<6)|a[1]&63;
        else if ((*a&240)==224)
            //Byte represents the start of an encoded character in the range
            //U+07FF to U+FFFF
            res[b]=((*a&15)<<12)|((a[1]&63)<<6)|a[2]&63;
        else if ((*a&248)==240) {
            //Byte represents the start of an encoded character beyond the
            //U+FFFF limit of 16-bit integers
            res[b]='?';
        }
        b++;
    }
    return res;
}

bool CheckFileExtension(const char* strfilename,const char* strext) {
    bool ret =false;
    while(*strfilename++ !='\0') {
        if( strcmp(strfilename,strext) == 0 ) {
            ret = true;
            break;
        }
    }
    return ret;
}

void GetFilenameNoExt(char* filenamenoext,const char* filename) {
    unsigned int i = 0;
    while(*filename !='\0') {
        if(*filename == '.' ) {
            filenamenoext[i] = '\0';
            break;
        }
        filenamenoext[i] = filename[0] ;
        *filename++;
        i++;
    }
}

int RandomRange(int low, int hight )	{
    return (low + (rand() % (hight-low)));
}

float RandomRange(float low, float hight )	{
#define R_MUL 1000
    if(low==hight)
        return hight;
    float val = (float) RandomRange( (int)(low*R_MUL), (int)(hight*R_MUL) );
    val = val / R_MUL;
    return val;
}

