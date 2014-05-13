#include "pch.h"
#include "XDSoundSystem.h"

#if WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
extern unsigned int g_Sound_AddSound(const std::string& soundfile);
extern bool g_Sound_PlaySound(unsigned int index);
extern bool g_Sound_StopSound(unsigned int index);
extern bool g_Sound_IsSoundPlaying(unsigned int index);
extern unsigned int g_Sound_GetSoundCount();
extern void g_Sound_Suspend();
extern void g_Sound_Resume();
#endif

XDSoundSystem* XDSoundSystem::m_Instance = nullptr;

XDSoundSystem& XDSoundSystem::GetInstance() {
    if(nullptr == m_Instance) {
        m_Instance = new XDSoundSystem();
    }
    return *m_Instance;
}
unsigned int XDSoundSystem::AddSound(const std::string& soundfile) {
#if WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
    return g_Sound_AddSound( soundfile );
#else
    //
#endif
}

bool XDSoundSystem::PlaySound(unsigned int index) {
#if WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
    return g_Sound_PlaySound( index );
#else
    //
#endif
}

bool XDSoundSystem::StopSound(unsigned int index) {
#if WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
    return g_Sound_StopSound( index );
#else
    //
#endif
}

bool XDSoundSystem::IsSoundPlaying(unsigned int index) const {
#if WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
    return g_Sound_IsSoundPlaying( index );
#else
    //
#endif
}

unsigned int XDSoundSystem::GetSoundCount() const {
#if WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
    return g_Sound_GetSoundCount();
#else
    //
#endif
}

void XDSoundSystem::Suspend() {
#if WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
    g_Sound_Suspend();
#else
    //
#endif
}

void XDSoundSystem::Resume() {
#if WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
    g_Sound_Resume();
#else
    //
#endif
}





