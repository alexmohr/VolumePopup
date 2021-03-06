#include "VolumeManager.h"

VolumeManager::VolumeManager()
{
}


VolumeManager::~VolumeManager()
{
}

bool VolumeManager::SetVolume(double nVolume, bool bScalar)
{

	HRESULT hr = NULL;
	IAudioEndpointVolume* endpointVolume = GetEndpointVolume();
	if (endpointVolume == NULL)
	{
		return false;
	}

	if (bScalar == false)
	{
		hr = endpointVolume->SetMasterVolumeLevel((float)nVolume / 100, NULL);
	}
	else if (bScalar == true)
	{
		hr = endpointVolume->SetMasterVolumeLevelScalar((float)nVolume, NULL);
	}
	endpointVolume->Release();

	CoUninitialize();

	return true;
}

double VolumeManager::GetVolume()
{
	HRESULT hr = NULL;
	IAudioEndpointVolume* endpointVolume = GetEndpointVolume();
	if (endpointVolume == NULL)
	{
		return -1; 
	}

	float currentVolume = 0; 
	hr = endpointVolume->GetMasterVolumeLevelScalar(&currentVolume);


	
	endpointVolume->Release();

	CoUninitialize();

	return currentVolume*100;
}

IAudioEndpointVolume* VolumeManager::GetEndpointVolume()
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
	deviceEnumerator->Release();
	deviceEnumerator = NULL;
	if (hr != NO_ERROR)
	{
		return NULL; 
	}

	IAudioEndpointVolume *endpointVolume = NULL;
	hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume),
		CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&endpointVolume);
	defaultDevice->Release();
	defaultDevice = NULL;

	return endpointVolume;
}
