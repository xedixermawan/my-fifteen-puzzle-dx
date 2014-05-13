
#pragma once

#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN
#endif

#if !defined(NOMINMAX)
#define NOMINMAX
#endif

#if WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP
#include <d3d11.h>
#elif WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
#include <d3d11_1.h>
#endif

#include <DirectXMath.h>

#include <algorithm>
#include <array>
#include <exception>
#include <malloc.h>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>
#include <assert.h>
#include <memory>

#pragma warning(push)
#pragma warning(disable : 4005)
#include <stdint.h>
#include <wrl.h>
#pragma warning(pop)