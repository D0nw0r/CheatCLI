// clang-format off
#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
// clang-format on

#include <string>
#include <vector>

class MemoryScanner {
private:
  HANDLE processHandle;
  DWORD processId;
  DWORD errorCode = 0;

public:
  enum ErrorCode {
    OPEN_PROCESS_FAILED = 1,
    PROCESS_NOT_FOUND = 2,
    SNAPSHOT_FAILED = 3,
  };
  bool attachToProcess(const std::string &processName);

  std::vector<uintptr_t> scanForValue(int targetValue);

  template <typename T>
  bool MemoryScanner::readMemory(uintptr_t address, T &value) {
    SIZE_T bytesRead;
    return ReadProcessMemory(processHandle, reinterpret_cast<LPVOID>(address),
                             &value, sizeof(T), &bytesRead);
  };

  void MemoryScanner::Rescan(int value, std::vector<uintptr_t> &addresses_vec);

  DWORD getProcId() { return processId; };
  INT getErrorCode() { return errorCode; };

  ~MemoryScanner() {
    if (processHandle && processHandle != INVALID_HANDLE_VALUE) {
      CloseHandle(processHandle);
    }
  }
};
