#ifndef __GUI_MANAGER_H__
#define __GUI_MANAGER_H__

#include <string>

class XDSoundSystem {
  public:
    static XDSoundSystem& GetInstance();
    unsigned int AddSound(const std::string& soundfile);
    bool PlaySound(unsigned int index);
    bool StopSound(unsigned int index);
    bool IsSoundPlaying(unsigned int index) const;
    unsigned int GetSoundCount() const;
    void Suspend();
    void Resume();
  private:
    static XDSoundSystem* m_Instance;
    XDSoundSystem() {};
    ~XDSoundSystem() {};
};

#endif // __GUI_MANAGER_H__