/*
*  (c) 2013 XediXermawan < edi.ermawan@gmail.com >
*/
#include <iostream>
#include <array>

class FileSystem {
  public:
    static std::string RWFolder;
    static std::string ROFolder;
    static unsigned char* GetByteArrayFromFile(std::string filename,unsigned int& length);
    static void Get_RWFolder(std::string& fullpath,const std::string& filename);
    static void Get_ROFolder(std::string& fullpath,const std::string& filename);
    static bool FileExist(const std::string& fullpath);
};