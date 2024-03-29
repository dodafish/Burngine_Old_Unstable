/************************************************************************************

Filename    :   OVR_Win32_HIDDevice.h
Content     :   Win32 HID device implementation.
Created     :   February 22, 2013
Authors     :   Lee Cooper

Copyright   :   Copyright 2013 Oculus VR, Inc. All Rights reserved.

Licensed under the Oculus VR SDK License Version 2.0 (the "License"); 
you may not use the Oculus VR SDK except in compliance with the License, 
which is provided at the time of installation or download, or which 
otherwise accompanies this software in either electronic or hard copy form.

You may obtain a copy of the License at

http://www.oculusvr.com/licenses/LICENSE-2.0 

Unless required by applicable law or agreed to in writing, the Oculus VR SDK 
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*************************************************************************************/

#ifndef OVR_Win32_HIDDevice_h
#define OVR_Win32_HIDDevice_h

#include "OVR_HIDDevice.h"
#include "OVR_Win32_DeviceManager.h"

#ifndef WINVER
  #define WINVER 0x0501
#endif // WINVER
#ifndef _WIN32_WINNT
  #define _WIN32_WINNT 0x0501
#endif // _WIN32_WINNT
#include <windows.h>
#include <setupapi.h>

#ifdef __MINGW32__
#include <ddk/ntstatus.h>
#include <ddk/ntapi.h>
#include <ddk/ntifs.h>
#endif //__MINGW32__

//-------------------------------------------------------------------------------------
// Define needed "hidsdi.h" functionality to avoid requiring DDK installation.
// #include "hidsdi.h"

#ifndef _HIDSDI_H
#define _HIDSDI_H
#include <pshpack4.h>

#define HIDP_STATUS_SUCCESS (0x11 << 16)
struct HIDP_PREPARSED_DATA;

struct HIDD_ATTRIBUTES
{
    ULONG   Size; // = sizeof (struct _HIDD_ATTRIBUTES)
    USHORT  VendorID;
    USHORT  ProductID;
    USHORT  VersionNumber;
};

struct HIDP_CAPS
{
    USHORT   Usage;
    USHORT   UsagePage;
    USHORT   InputReportByteLength;
    USHORT   OutputReportByteLength;
    USHORT   FeatureReportByteLength;
    USHORT   Reserved[17];

    USHORT   NumberLinkCollectionNodes;
    USHORT   NumberInputButtonCaps;
    USHORT   NumberInputValueCaps;
    USHORT   NumberInputDataIndices;
    USHORT   NumberOutputButtonCaps;
    USHORT   NumberOutputValueCaps;
    USHORT   NumberOutputDataIndices;
    USHORT   NumberFeatureButtonCaps;
    USHORT   NumberFeatureValueCaps;
    USHORT   NumberFeatureDataIndices;
};

#include <poppack.h>
#endif


namespace OVR { namespace Win32 { 

class HIDDeviceManager;
class DeviceManager;

//-------------------------------------------------------------------------------------
// ***** Win32 HIDDevice

class HIDDevice : public OVR::HIDDevice, public DeviceManagerThread::Notifier
{
public:

    HIDDevice(HIDDeviceManager* manager);

    // This is a minimal constructor used during enumeration for us to pass
    // a HIDDevice to the visit function (so that it can query feature reports). 
    HIDDevice(HIDDeviceManager* manager, HANDLE device);

    ~HIDDevice();

    bool HIDInitialize(const String& path);
    void HIDShutdown();

    // OVR::HIDDevice
	bool SetFeatureReport(UByte* data, UInt32 length);
	bool GetFeatureReport(UByte* data, UInt32 length);
    

    // DeviceManagerThread::Notifier
    void OnOverlappedEvent(HANDLE hevent);
    UInt64 OnTicks(UInt64 ticksMks);
    bool OnDeviceMessage(DeviceMessageType messageType, const String& devicePath, bool* error);

private:
    bool openDevice();
    bool initInfo();
    bool initializeRead();
    bool processReadResult();
    void closeDevice();
    void closeDeviceOnIOError();

    bool                inMinimalMode;
    HIDDeviceManager*   HIDManager;
	HANDLE              Device;
    HIDDeviceDesc       DevDesc; 

    OVERLAPPED          ReadOverlapped;
    bool                ReadRequested;

    enum { ReadBufferSize = 96 };
    UByte               ReadBuffer[ReadBufferSize];

    UInt16              InputReportBufferLength;
    UInt16              OutputReportBufferLength;
    UInt16              FeatureReportBufferLength;
};

//-------------------------------------------------------------------------------------
// ***** Win32 HIDDeviceManager

class HIDDeviceManager : public OVR::HIDDeviceManager
{
	friend class HIDDevice;
public:

	HIDDeviceManager(DeviceManager* manager);
    virtual ~HIDDeviceManager();

    virtual bool Initialize();
    virtual void Shutdown();

    virtual bool Enumerate(HIDEnumerateVisitor* enumVisitor);
    virtual OVR::HIDDevice* Open(const String& path);

    // Fills HIDDeviceDesc by using the path.
    // Returns 'true' if successful, 'false' otherwise.
    bool GetHIDDeviceDesc(const String& path, HIDDeviceDesc* pdevDesc) const;

    GUID GetHIDGuid() { return HidGuid; }
    
    static HIDDeviceManager* CreateInternal(DeviceManager* manager);

private:

    DeviceManager* Manager;     // Back pointer can just be a raw pointer.

    HMODULE hHidLib;
    GUID    HidGuid;

    // Macros to declare and resolve needed functions from library.
#define OVR_DECLARE_HIDFUNC(func, rettype, args)   \
typedef rettype (OVR_STDCALL *PFn_##func) args;  \
PFn_##func      func;
#define OVR_RESOLVE_HIDFUNC(func) \
func = (PFn_##func)::GetProcAddress(hHidLib, #func)

    OVR_DECLARE_HIDFUNC(HidD_GetHidGuid,            void,    (GUID *hidGuid));
    OVR_DECLARE_HIDFUNC(HidD_SetNumInputBuffers,    BOOLEAN, (HANDLE hidDev, ULONG numberBuffers));
    OVR_DECLARE_HIDFUNC(HidD_GetFeature,            BOOLEAN, (HANDLE hidDev, PVOID buffer, ULONG bufferLength));
    OVR_DECLARE_HIDFUNC(HidD_SetFeature,            BOOLEAN, (HANDLE hidDev, PVOID buffer, ULONG bufferLength));
    OVR_DECLARE_HIDFUNC(HidD_GetAttributes,         BOOLEAN, (HANDLE hidDev, HIDD_ATTRIBUTES *attributes));
    OVR_DECLARE_HIDFUNC(HidD_GetManufacturerString, BOOLEAN, (HANDLE hidDev, PVOID buffer, ULONG bufferLength));
    OVR_DECLARE_HIDFUNC(HidD_GetProductString,      BOOLEAN, (HANDLE hidDev, PVOID buffer, ULONG bufferLength));
    OVR_DECLARE_HIDFUNC(HidD_GetSerialNumberString, BOOLEAN, (HANDLE hidDev, PVOID buffer, ULONG bufferLength));
    OVR_DECLARE_HIDFUNC(HidD_GetPreparsedData,      BOOLEAN, (HANDLE hidDev, HIDP_PREPARSED_DATA **preparsedData));
    OVR_DECLARE_HIDFUNC(HidD_FreePreparsedData,     BOOLEAN, (HIDP_PREPARSED_DATA *preparsedData));
    OVR_DECLARE_HIDFUNC(HidP_GetCaps,               NTSTATUS,(HIDP_PREPARSED_DATA *preparsedData, HIDP_CAPS* caps));

    HANDLE CreateHIDFile(const char* path, bool exclusiveAccess = true) const
    {
        return ::CreateFileA(path, GENERIC_WRITE|GENERIC_READ,
            (!exclusiveAccess) ? (FILE_SHARE_READ|FILE_SHARE_WRITE) : 0x0, 
            NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);
    }

    // Helper functions to fill in HIDDeviceDesc from open device handle.
    bool initVendorProductVersion(HANDLE hidDev, HIDDeviceDesc* desc) const;
    bool initUsage(HANDLE hidDev, HIDDeviceDesc* desc) const;
    void initStrings(HANDLE hidDev, HIDDeviceDesc* desc) const;

    bool getFullDesc(HANDLE hidDev, HIDDeviceDesc* desc) const;
};

}} // namespace OVR::Win32

#endif // OVR_Win32_HIDDevice_h
