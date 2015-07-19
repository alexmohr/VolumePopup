#pragma once
#include <mmdeviceapi.h>
#include <endpointvolume.h>


class VolumeManager
{
public:
	VolumeManager();
	~VolumeManager();

	bool SetVolume(double nVolume, bool bScalar); 
	double GetVolume();
private:
	// Do not forget to release the active x when done.
	IAudioEndpointVolume* GetEndpointVolume();
};

