#include "procexp_compat.h"

/* Correspond to original globals at 0x51E0, 0x51E8, 0x51F0, 0x5220, 0x5228. */
ULONG gBuildNumber;
POBJECT_TYPE gMutantObjectType;
PROCEXP_ACQUIRE_EXIT_SYNC gAcquireProcessExitSynchronization;
PROCEXP_RELEASE_EXIT_SYNC gReleaseProcessExitSynchronization;
PROCEXP_GET_POOL_SIZE gGetMaximumNonPagedPoolInBytes;

/* Original .data initializes 0x5218 to -1; DriverEntry replaces it on Win7+. */
PROCEXP_GET_OBJECT_TYPE gGetObjectType = (PROCEXP_GET_OBJECT_TYPE)(LONG_PTR)-1;
