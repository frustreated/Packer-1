#pragma once

#include <cstdint>

enum Win32SystemCall
{
	NtAllocateVirtualMemory,
	NtFreeVirtualMemory,
	NtProtectVirtualMemory,
	NtCreateFile,
	NtWriteFile,
	NtClose,
	NtCreateSection,
	NtMapViewOfSection,
	NtUnmapViewOfSection,
	NtQueryFullAttributesFile,
	NtSetInformationFile,
	NtFlushBuffersFile,
	NtFlushInstructionCache,
	NtTerminateProcess,

	SystemCallMax
};

class Win32SystemCaller
{
protected:
	const uint16_t *systemCalls_;
protected:
	Win32SystemCaller(const uint16_t *systemCalls) : systemCalls_(systemCalls) {}
	static uint32_t __cdecl executeWin32Syscall(uint32_t syscallno, uint32_t *argv);
	static uint32_t __cdecl executeWoW64Syscall(uint32_t syscallno, uint64_t *argv);
public:
	virtual bool freeVirtual(void *BaseAddress) = 0;
	virtual void *allocateVirtual(size_t DesiredAddress, size_t RegionSize, size_t AllocationType, size_t Protect) = 0;
	virtual void protectVirtual(void *BaseAddress, size_t NumberOfBytes, size_t NewAccessProtection, size_t *OldAccessProtection = nullptr) = 0;
	virtual void *createFile(uint32_t DesiredAccess, const wchar_t *Filename, size_t FilenameLength, size_t ShareAccess, size_t CreateDisposition) = 0;
	virtual size_t writeFile(void *fileHandle, const uint8_t *buffer, size_t bufferSize) = 0;
	virtual void flushFile(void *fileHandle) = 0;
	virtual void closeHandle(void *handle) = 0;
	virtual void *createSection(void *file, uint32_t flProtect, uint64_t sectionSize, wchar_t *lpName, size_t NameLength) = 0;
	virtual void *mapViewOfSection(void *section, uint32_t dwDesiredAccess, uint64_t offset, size_t dwNumberOfBytesToMap, size_t lpBaseAddress) = 0;
	virtual void unmapViewOfSection(void *lpBaseAddress) = 0;
	virtual uint32_t getFileAttributes(const wchar_t *filePath, size_t filePathLen) = 0;
	virtual void setFileSize(void *file, uint64_t size) = 0;
	virtual void flushInstructionCache(size_t offset, size_t size) = 0;
	virtual void terminate() = 0;

	static Win32SystemCaller *get(bool forceinit = false);
};

class Win32x86SystemCaller : public Win32SystemCaller
{
public:
	Win32x86SystemCaller(const uint16_t *systemCalls) : Win32SystemCaller(systemCalls) {}

	virtual bool freeVirtual(void *BaseAddress);
	virtual void *allocateVirtual(size_t DesiredAddress, size_t RegionSize, size_t AllocationType, size_t Protect);
	virtual void protectVirtual(void *BaseAddress, size_t NumberOfBytes, size_t NewAccessProtection, size_t *OldAccessProtection = nullptr);
	virtual void *createFile(uint32_t DesiredAccess, const wchar_t *Filename, size_t FilenameLength, size_t ShareAccess, size_t CreateDisposition);
	virtual size_t writeFile(void *fileHandle, const uint8_t *buffer, size_t bufferSize);
	virtual void flushFile(void *fileHandle);
	virtual void closeHandle(void *handle);
	virtual void *createSection(void *file, uint32_t flProtect, uint64_t sectionSize, wchar_t *lpName, size_t NameLength);
	virtual void *mapViewOfSection(void *section, uint32_t dwDesiredAccess, uint64_t offset, size_t dwNumberOfBytesToMap, size_t lpBaseAddress);
	virtual void unmapViewOfSection(void *lpBaseAddress);
	virtual uint32_t getFileAttributes(const wchar_t *filePath, size_t filePathLen);
	virtual void setFileSize(void *file, uint64_t size);
	virtual void flushInstructionCache(size_t offset, size_t size);
	virtual void terminate();
};

class Win32WOW64SystemCaller : public Win32SystemCaller
{
public:
	Win32WOW64SystemCaller(const uint16_t *systemCalls) : Win32SystemCaller(systemCalls) {}

	virtual bool freeVirtual(void *BaseAddress);
	virtual void *allocateVirtual(size_t DesiredAddress, size_t RegionSize, size_t AllocationType, size_t Protect);
	virtual void protectVirtual(void *BaseAddress, size_t NumberOfBytes, size_t NewAccessProtection, size_t *OldAccessProtection = nullptr);
	virtual void *createFile(uint32_t DesiredAccess, const wchar_t *Filename, size_t FilenameLength, size_t ShareAccess, size_t CreateDisposition);
	virtual size_t writeFile(void *fileHandle, const uint8_t *buffer, size_t bufferSize);
	virtual void flushFile(void *fileHandle);
	virtual void closeHandle(void *handle);
	virtual void *createSection(void *file, uint32_t flProtect, uint64_t sectionSize, wchar_t *lpName, size_t NameLength);
	virtual void *mapViewOfSection(void *section, uint32_t dwDesiredAccess, uint64_t offset, size_t dwNumberOfBytesToMap, size_t lpBaseAddress);
	virtual void unmapViewOfSection(void *lpBaseAddress);
	virtual uint32_t getFileAttributes(const wchar_t *filePath, size_t filePathLen);
	virtual void setFileSize(void *file, uint64_t size);
	virtual void flushInstructionCache(size_t offset, size_t size);
	virtual void terminate();
};


#define GENERIC_READ                     (0x80000000L)
#define GENERIC_WRITE                    (0x40000000L)
#define GENERIC_EXECUTE                  (0x20000000L)
#define GENERIC_ALL                      (0x10000000L)

#define FILE_SHARE_READ                 0x00000001  
#define FILE_SHARE_WRITE                0x00000002  
#define FILE_SHARE_DELETE               0x00000004  

#define FILE_SUPERSEDE                  0x00000000
#define FILE_OPEN                       0x00000001
#define FILE_CREATE                     0x00000002
#define FILE_OPEN_IF                    0x00000003
#define FILE_OVERWRITE                  0x00000004
#define FILE_OVERWRITE_IF               0x00000005
#define FILE_MAXIMUM_DISPOSITION        0x00000005

#define PAGE_NOACCESS          0x01     
#define PAGE_READONLY          0x02     
#define PAGE_READWRITE         0x04     
#define PAGE_WRITECOPY         0x08     
#define PAGE_EXECUTE           0x10     
#define PAGE_EXECUTE_READ      0x20     
#define PAGE_EXECUTE_READWRITE 0x40     
#define PAGE_EXECUTE_WRITECOPY 0x80   

#define INVALID_HANDLE_VALUE reinterpret_cast<void *>(-1)

#define SECTION_QUERY                0x0001
#define SECTION_MAP_WRITE            0x0002
#define SECTION_MAP_READ             0x0004
#define SECTION_MAP_EXECUTE          0x0008
#define SECTION_EXTEND_SIZE          0x0010
#define SECTION_MAP_EXECUTE_EXPLICIT 0x0020 // not included in SECTION_ALL_ACCESS

#define SECTION_ALL_ACCESS (STANDARD_RIGHTS_REQUIRED|SECTION_QUERY|\
	SECTION_MAP_WRITE |      \
	SECTION_MAP_READ |       \
	SECTION_MAP_EXECUTE |    \
	SECTION_EXTEND_SIZE)

#define SESSION_QUERY_ACCESS  0x0001
#define SESSION_MODIFY_ACCESS 0x0002

#define SESSION_ALL_ACCESS (STANDARD_RIGHTS_REQUIRED |  \
	SESSION_QUERY_ACCESS |             \
	SESSION_MODIFY_ACCESS)

#define FILE_MAP_WRITE      SECTION_MAP_WRITE
#define FILE_MAP_READ       SECTION_MAP_READ
#define FILE_MAP_ALL_ACCESS SECTION_ALL_ACCESS
#define FILE_MAP_EXECUTE    SECTION_MAP_EXECUTE_EXPLICIT    // not included in FILE_MAP_ALL_ACCESS
#define FILE_MAP_COPY       0x00000001
#define FILE_MAP_RESERVE    0x80000000

#define INVALID_FILE_ATTRIBUTES ((uint32_t)-1)
#define SEC_FILE           0x800000     
#define SEC_IMAGE         0x1000000     
#define SEC_PROTECTED_IMAGE  0x2000000  
#define SEC_RESERVE       0x4000000     
#define SEC_COMMIT        0x8000000     
#define SEC_NOCACHE      0x10000000     
#define SEC_WRITECOMBINE 0x40000000     
#define SEC_LARGE_PAGES  0x80000000     

#define MEM_COMMIT                  0x1000      
#define MEM_RESERVE                 0x2000      
#define MEM_DECOMMIT                0x4000      
#define MEM_RELEASE                 0x8000      
