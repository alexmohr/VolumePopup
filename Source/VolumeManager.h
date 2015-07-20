#pragma once
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <functional>

// Help on how to implement this class has been taken from XBMC@https://dev.kudryavka.moe:3001/hayandev/XBMC/blob/ce69e5e2010b9eb0187f023bebfce3b5fe69da41/xbmc/win32/IMMNotificationClient.h
//

typedef std::function<void(float)> VolumeChangedCallback;


class VolumeManager : public IMMNotificationClient, public  IAudioEndpointVolumeCallback

{
private:
		LONG _cRef;
		IMMDeviceEnumerator *_pEnumerator;

		PROPERTYKEY PK_VOLUME_CHANGED; 

		IMMDeviceEnumerator* _masterDevice; 
		IAudioEndpointVolume* _mastersEndPointVolume; 

		VolumeChangedCallback _vcb; 

	public:

		void GetMasterAndRegister()
		{
			HRESULT hr = NULL;
			bool decibels = false;
			bool scalar = false;


			CoInitialize(NULL);
			IMMDeviceEnumerator *deviceEnumerator = NULL;
			hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER,
				__uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator);
			IMMDevice *defaultDevice = NULL;

			hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
			if (hr != NO_ERROR)
			{
				// todo handle this shit
			}
			deviceEnumerator->RegisterEndpointNotificationCallback(this);
			_masterDevice = deviceEnumerator; 

	
			_mastersEndPointVolume = NULL;
			defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&_mastersEndPointVolume);
			defaultDevice->Release();
			defaultDevice = NULL;
		}

		VolumeManager() : _cRef(1), _pEnumerator(NULL)
		{
			PSPropertyKeyFromString(L"{9855C4CD-DF8C-449C-A181-8191B68BD06C} 0", &PK_VOLUME_CHANGED);
			GetMasterAndRegister();
		}

		~VolumeManager()
		{
			// Todo release it
			//SAFE_RELEASE(_pEnumerator);
		}

		void ConnectCallback(VolumeChangedCallback cb)
		{
			_vcb = cb;
		}


		float GetMasterVolume()
		{
			float currentVolume = 0;
			_mastersEndPointVolume->GetMasterVolumeLevelScalar(&currentVolume);
			_mastersEndPointVolume->AddRef();
			_mastersEndPointVolume->RegisterControlChangeNotify(this);
			return currentVolume*100;
		}

		

		ULONG STDMETHODCALLTYPE AddRef()
		{
			return InterlockedIncrement(&_cRef);
		}

		ULONG STDMETHODCALLTYPE Release()
		{
			ULONG ulRef = InterlockedDecrement(&_cRef);
			if (0 == ulRef)
			{
				delete this;
			}
			return ulRef;
		}

		HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, VOID **ppvInterface)
		{
			return S_OK;

			/*if (IID_IUnknown == riid)
			{
				AddRef();
				*ppvInterface = (IMMNotificationClient::IUnknown*)this;
			}
			else if (__uuidof(IMMNotificationClient) == riid)
			{
				AddRef();
				*ppvInterface = (IMMNotificationClient*)this;
			}
			else
			{
				*ppvInterface = NULL;
				return E_NOINTERFACE;
			}
			return S_OK;*/
		}

		HRESULT STDMETHODCALLTYPE OnNotify(PAUDIO_VOLUME_NOTIFICATION_DATA pNotify)
		{
			_vcb(pNotify->fMasterVolume*100); 
			return S_OK;
		}

		// Callback methods for device-event notifications.
#pragma region Unused
		HRESULT STDMETHODCALLTYPE OnDefaultDeviceChanged(EDataFlow flow, ERole role, LPCWSTR pwstrDeviceId)
		{
			_masterDevice->UnregisterEndpointNotificationCallback(this); 
			_masterDevice->Release(); 
			_mastersEndPointVolume->Release(); 

			GetMasterAndRegister(); 
			return S_OK;
		}

		HRESULT STDMETHODCALLTYPE OnDeviceAdded(LPCWSTR pwstrDeviceId)
		{
		
			return S_OK;
		}

		HRESULT STDMETHODCALLTYPE OnDeviceRemoved(LPCWSTR pwstrDeviceId)
		{
			
			return S_OK;
		}

		HRESULT STDMETHODCALLTYPE OnDeviceStateChanged(LPCWSTR pwstrDeviceId, DWORD dwNewState)
		{
			return S_OK;
		}
#pragma endregion

		HRESULT STDMETHODCALLTYPE OnPropertyValueChanged(LPCWSTR pwstrDeviceId, const PROPERTYKEY key)
		{
			if (PK_VOLUME_CHANGED == key)
			{
				_vcb(GetMasterVolume());
			}
			return S_OK;
		}
	};

