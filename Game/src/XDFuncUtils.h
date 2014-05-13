/*
*  (c) 2013 XediXermawan < edi.ermawan@gmail.com >
*/
#ifndef __FUNC_UTIL_H__
#define __FUNC_UTIL_H__

#include <cstring>
#include <sstream>


wchar_t* UTF8ToWChar(const char* str);
bool CheckFileExtension(const char* strfilename,const char* strext);
void GetFilenameNoExt(char* filenamenoext,const char* filename);
int RandomRange(int low, int hight );
float RandomRange(float low, float hight );

template < class T >
void StrToNumber(std::string& datastr,T& out) {
    std::istringstream buffer(datastr);
    buffer >> out;
}

#endif // __FUNC_UTIL_H__