/*
*  (c) 2013 XediXermawan < edi.ermawan@gmail.com >
*/
#ifndef __GUI_MANAGER_H__
#define __GUI_MANAGER_H__

#include "XDElementBase.h"

namespace gui {

typedef std::vector<std::shared_ptr<ElementBase>> GUIElementArray;

class gui_page_s  {
  public:
    gui_page_s(const std::string& filename) {
        Name = filename;
    };
    ~gui_page_s() {};
    std::string Name;
    unsigned int Width;
    unsigned int Height;
    GUIElementArray ElementArray;
};

typedef std::vector<std::shared_ptr<gui_page_s>> GUIPageArray;

class gui_file_s  {
  public:
    gui_file_s(const std::string& filename) {
        Name = filename;
    };
    ~gui_file_s() {};
    std::string Name;
    GUIPageArray PageArray;
};

typedef std::vector<std::shared_ptr<gui_file_s>> GUIFileArray;

class GUIManager {
  public:
    static GUIManager& GetInstance();
    void AddElement(const std::shared_ptr < ElementBase >& element);
    void RemoveElement(const std::shared_ptr < ElementBase >& element);
    void CheckIntersect(const int& posx,const int& posy);
    void CheckClicked(const int& posx,const int& posy);
    void Clicked(const int& posx,const int& posy);
    void Update(const double delta_time);
    void Render();
    void HandleInput();
    void Clear();
    void Load(const std::string& guifile);
    static void Free();
  private:
    static GUIManager* m_Instance;
    GUIElementArray m_Elements;
    GUIPageArray m_GUIPages;
    GUIFileArray m_GUIFiles;
    GUIManager();
    ~GUIManager();
};

};

#endif // __GUI_MANAGER_H__