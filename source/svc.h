#pragma once

void svc_sleepThread(s64 ns);
s32 svc_connectToPort(Handle* out, const char* portName);
s32 svc_sendSyncRequest(Handle session);
s32 svc_getProcessId(u32 *out, Handle handle);
