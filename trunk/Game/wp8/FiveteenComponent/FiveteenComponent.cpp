#include "pch.h"
#include "FiveteenComponent.h"
#include "Direct3DContentProvider.h"
#include <mutex>
#include "XAudio2SoundPlayer.h"
#include "SoundFileReader.h"
#include <fstream>

using namespace Windows::Foundation;
using namespace Windows::UI::Core;
using namespace Microsoft::WRL;
using namespace Windows::Phone::Graphics::Interop;
using namespace Windows::Phone::Input::Interop;

double g_fps_measure;
bool g_toggle_line;
bool g_toogle_graphics;
std::string g_rw_local_folder;
std::string g_ro_local_folder;
std::mutex g_tmutex;
float g_screen_w;
float g_screen_h;
std::vector< SINPUT > m_Inputs;
XAudio2SoundPlayer* m_soundPlayer = nullptr;

unsigned int g_Sound_AddSound(const std::string& soundfile)
{
	unsigned int retval=0;
	if( m_soundPlayer) 
	{
		try
		{
			std::wstring wstr=L"";
			wstr.assign( soundfile.begin(), soundfile.end() );
			Platform::String^ pstring = ref new Platform::String( wstr.data() );
			SoundFileReader nextSound( pstring );
			(void)m_soundPlayer->AddSound( nextSound.GetSoundFormat(), nextSound.GetSoundData() );
			retval =1;
		}
		catch (Platform::FailureException^)
		{
			retval=0;
		}
	}
	return retval;
}

bool g_Sound_PlaySound(unsigned int index)
{
	if( m_soundPlayer)
		return m_soundPlayer->PlaySound( index );
	return 0;
}

bool g_Sound_StopSound(unsigned int index)
{
	if( m_soundPlayer)
		return m_soundPlayer->StopSound( index );
	return 0;
}

bool g_Sound_IsSoundPlaying(unsigned int index)
{
	if( m_soundPlayer)
		return m_soundPlayer->IsSoundPlaying( index );
	return 0;
}

unsigned int g_Sound_GetSoundCount()
{
	if( m_soundPlayer)
		return m_soundPlayer->GetSoundCount();
	return 0;
}

void g_Sound_Suspend()
{
	if( m_soundPlayer)
		m_soundPlayer->Suspend();
}

void g_Sound_Resume()
{
	if( m_soundPlayer)
		m_soundPlayer->Resume();
}

unsigned char* GetByteArrayFromFile(std::string filename,unsigned int& length)  {
	unsigned char* f_bytearray = NULL;
	// open the file
	std::ifstream VertexFile(filename, std::ios::in | std::ios::binary | std::ios::ate);

	// if open was successful
	bool is_open_ok = VertexFile.is_open() ;
	assert( is_open_ok ) ;
	if(is_open_ok) {
		// find the length of the file
		int f_length = (int)VertexFile.tellg();
		length = f_length;
		// collect the file data
		f_bytearray = new unsigned char[f_length];
		VertexFile.seekg(0, std::ios::beg);
		VertexFile.read(reinterpret_cast<char*>(f_bytearray), f_length);
		VertexFile.close();
	}
	return f_bytearray;
}

namespace PhoneDirect3DXamlAppComponent
{

Direct3DBackground::Direct3DBackground() :
	m_timer(ref new BasicTimer()),
	m_AppState( State::INIT )
{
	Game = new XDGame();
	m_TouchState = -1;
	m_TouchID    = -1;
	m_BackKey    = 0;
}

IDrawingSurfaceBackgroundContentProvider^ Direct3DBackground::CreateContentProvider()
{
	ComPtr<Direct3DContentProvider> provider = Make<Direct3DContentProvider>(this);
	return reinterpret_cast<IDrawingSurfaceBackgroundContentProvider^>(provider.Detach());
}

// IDrawingSurfaceManipulationHandler
void Direct3DBackground::SetManipulationHost(DrawingSurfaceManipulationHost^ manipulationHost)
{
	manipulationHost->PointerPressed +=
		ref new TypedEventHandler<DrawingSurfaceManipulationHost^, PointerEventArgs^>(this, &Direct3DBackground::OnPointerPressed);

	manipulationHost->PointerMoved +=
		ref new TypedEventHandler<DrawingSurfaceManipulationHost^, PointerEventArgs^>(this, &Direct3DBackground::OnPointerMoved);

	manipulationHost->PointerReleased +=
		ref new TypedEventHandler<DrawingSurfaceManipulationHost^, PointerEventArgs^>(this, &Direct3DBackground::OnPointerReleased);
}

// Event Handlers
void Direct3DBackground::OnPointerPressed(DrawingSurfaceManipulationHost^ sender, PointerEventArgs^ args)
{
	g_tmutex.lock();
	if( m_TouchState == -1 &&  m_TouchID ==-1 )
	{
		m_TouchState = 1;
		m_TouchID    = args->CurrentPoint->PointerId;
		// Insert your code here.
		SINPUT input;
		input.pposx =   args->CurrentPoint->Position.X;
		input.pposy =   args->CurrentPoint->Position.Y;
		input.pflag =   FINPUT::P_PRESSED;
		m_Inputs.push_back( input );
		//Game->OnHandleInput(input);
	}
	g_tmutex.unlock();
}

void Direct3DBackground::OnPointerMoved(DrawingSurfaceManipulationHost^ sender, PointerEventArgs^ args)
{
	g_tmutex.lock();
	if( m_TouchState == 1 && m_TouchID == args->CurrentPoint->PointerId )
	{
		SINPUT input;
		input.pposx =   args->CurrentPoint->Position.X;
		input.pposy =   args->CurrentPoint->Position.Y;
		input.pflag =   FINPUT::P_MOVE;
		m_Inputs.push_back( input );
	}
	g_tmutex.unlock();
}

void Direct3DBackground::OnPointerReleased(DrawingSurfaceManipulationHost^ sender, PointerEventArgs^ args)
{
	g_tmutex.lock();
	if( m_TouchState == 1 && m_TouchID == args->CurrentPoint->PointerId )
	{
		m_TouchState =-1;
		m_TouchID    =-1;
		SINPUT input;
		input.pposx =   args->CurrentPoint->Position.X;
		input.pposy =   args->CurrentPoint->Position.Y;
		input.pflag =   FINPUT::P_RELEASED;
		m_Inputs.push_back( input );
	}
	g_tmutex.unlock();
}

// Interface With Direct3DContentProvider
HRESULT Direct3DBackground::Connect(_In_ IDrawingSurfaceRuntimeHostNative* host, _In_ ID3D11Device1* device)
{
	// ro
    auto current = Windows::ApplicationModel::Package::Current;
    auto folder  = current->InstalledLocation;
    auto path    = folder->Path;	
	path        +="\\data\\";
    std::string path2(path->Begin(), path->End());
    g_ro_local_folder = path2;

	// rw
	Platform::String^ str = Windows::Storage::ApplicationData::Current->LocalFolder->Path;
	std::string path3(str->Begin(), str->End());
    g_rw_local_folder =  path3;
	m_timer->Reset();

	return S_OK;
}

void Direct3DBackground::Disconnect()
{
	Game->OnSuspending();
	m_AppState = State::RESUMING;
	g_Sound_Suspend();
}

HRESULT Direct3DBackground::PrepareResources(_In_ const LARGE_INTEGER* presentTargetTime, _Inout_ DrawingSurfaceSizeF* desiredRenderTargetSize)
{
	m_timer->Update();
	desiredRenderTargetSize->width  = 480; // RenderResolution.Width;
	desiredRenderTargetSize->height = 800; // RenderResolution.Height;
	g_screen_w = desiredRenderTargetSize->width;
	g_screen_h = desiredRenderTargetSize->height;
	return S_OK;
}

HRESULT Direct3DBackground::Draw(_In_ ID3D11Device1* device, _In_ ID3D11DeviceContext1* context, _In_ ID3D11RenderTargetView* renderTargetView)
{
	g_tmutex.lock();
		if( !m_Inputs.empty() )
		{
			Game->OnHandleInput( m_Inputs[0] );
			m_Inputs.erase( m_Inputs.begin() );
		}
	g_tmutex.unlock();
	if( m_BackKey )
	{
		m_BackKey = 0;
		SINPUT input;
		input.pflag = FINPUT::KEY_DOWN;
		input.pid   = 27;
		Game->OnHandleInput( input );
	}
	switch( m_AppState ) 
	{
	case State::INIT :
		{
			Game->Initialize( device, context  );
			// init sound
			const uint32 SOUND_SAMPLE_RATE = 48000;
			try
			{
				m_soundPlayer = new XAudio2SoundPlayer(SOUND_SAMPLE_RATE);
			}
			catch (Platform::Exception^)
			{
				m_soundPlayer = nullptr;
			}
			std::vector <std::string > sfile ;
			sfile.push_back("electro_adpcm.wav");
			sfile.push_back("hiphoppy_adpcm.wav");
			sfile.push_back("musicmono_adpcm.wav");
			sfile.push_back("techno_adpcm.wav");
			for (unsigned int index = 0; index < sfile.size() ; index++)
			{
				g_Sound_AddSound( sfile[ index ] );
			}
			// next substate
			m_AppState = State::RUNNING;
		}
		break;
	case State::RUNNING :
		Game->Update( m_timer->Delta, device, context,renderTargetView  );
		break;
	case State::RESUMING :
		g_Sound_Resume();

		Game->Initialize( device, context  );
		Game->OnResuming();
		m_AppState = State::RUNNING;
		break;
	case State::PAUSE :
		break;
	default:
		break;
	};
	RequestAdditionalFrame();

	return S_OK;
}

void Direct3DBackground::OnBackKeyPress()
{
	m_BackKey = 1;
}

}