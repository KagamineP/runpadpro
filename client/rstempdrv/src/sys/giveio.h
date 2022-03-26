/////////////////////////////////////////////////////////////////////////
// ����		: giveio.h
// �������� : ������� ����������� ����������� ���������� � �������
// OS		: Windows NT 
/////////////////////////////////////////////////////////////////////////

#include <ntddk.h>
#include "gpioctl.h"        // IOCTL ���������

#if     !defined(__GIVEIO_H__)
#define __GIVEIO_H__

// NT device name
#define GPD_DEVICE_NAME L"\\Device\\giveio"

// ��� ���������� �������� �������.   ��� �������� �������� �������� CreateFile
// ���������� ��� ���������� "\\.\GiveIO" ��� � C\C++ �����������: "\\\\.\\GiveIO".

#define DOS_DEVICE_NAME L"\\DosDevices\\giveio"

#define PORTIO_TAG 'TROP'

// ��������� ��������� ������ �������� ���������� �� ������ ������ ����������

typedef struct _LOCAL_DEVICE_INFO {
    PVOID               PortBase;			// ������� ����� �����
    ULONG               PortCount;			// ���������� ������������ I/O �������.
    ULONG               PortMemoryType;		// HalTranslateBusAddress MemoryType
    PDEVICE_OBJECT      DeviceObject;		// GiveIO ������ ����������
    PDEVICE_OBJECT      NextLowerDriver;    // �������� �����
    BOOLEAN             Started;
    BOOLEAN             Removed;
    BOOLEAN             PortWasMapped;		
    BOOLEAN             Filler[1]; 			
    IO_REMOVE_LOCK      RemoveLock;
} LOCAL_DEVICE_INFO, *PLOCAL_DEVICE_INFO;


#if DBG
#define DebugPrint(_x_) \
               DbgPrint ("GiveIO:"); \
               DbgPrint _x_;

#define TRAP() DbgBreakPoint()

#else
#define DebugPrint(_x_)
#define TRAP()
#endif


NTSYSAPI
NTSTATUS
NTAPI
    Ke386QueryIoAccessMap(ULONG dwFlag,PVOID pIopm);


NTSYSAPI
NTSTATUS 
NTAPI
    Ke386SetIoAccessMap(ULONG dwFlag,PVOID pIopm);

NTSYSAPI
NTSTATUS 
NTAPI
    Ke386IoSetAccessProcess(PEPROCESS pProcess, ULONG dwFlag);

/********************* ��������� ������� ***********************************/
//

NTSTATUS    
DriverEntry(       
    IN  PDRIVER_OBJECT DriverObject,
    IN  PUNICODE_STRING RegistryPath 
    );

NTSTATUS
CreateCloseDispatch(
         IN  PDRIVER_OBJECT DriverObject,
         IN  PIRP pIrp);

NTSTATUS
ADriverUnload(
         IN  PDRIVER_OBJECT DriverObject);

#endif


