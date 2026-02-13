
#pragma once

	enum MessageType {
	   None = 0,
	   PropertyChanged = 1,
	   SetPropertyRequest = 2,
	   GetPropertyRequest = 3,
	   GetPropertyResponse = 4,

	   AudioStreamEnable = 5,
	   IqStreamEnable = 6, 
	   SpectrumEnable = 7,
	   DeviceStreamEnable = 8,
	   SelectedDeviceIndex = 9,
	   StartRecording = 10,
	   StopRecording = 11,
	   ApplyDeviceProfile = 12
	};

