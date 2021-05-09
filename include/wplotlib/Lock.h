#pragma once
#define _WINSOCKAPI_ /* Prevent inclusion of winsock.h in windows.h */
#include <Windows.h>

class Lock {
public:
  Lock(PSRWLOCK lock) : _plock(lock) { ::AcquireSRWLockExclusive(_plock); }
  ~Lock() { ::ReleaseSRWLockExclusive(_plock); }

private:
  PSRWLOCK _plock;
};