#include "scanner.h"
#include <vector>
#include <winnt.h>

bool MemoryScanner::attachToProcess(const std::string &processName) {
  HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (snapshot == INVALID_HANDLE_VALUE) {
    errorCode = SNAPSHOT_FAILED;
    return false;
  }

  PROCESSENTRY32 entry;
  entry.dwSize = sizeof(PROCESSENTRY32);

  if (Process32First(snapshot, &entry)) {
    do {
      // Convert to string for comparison
      std::wstring wExeName(entry.szExeFile);
      std::string exeName(wExeName.begin(), wExeName.end());

      if (processName == exeName) {
        processId = entry.th32ProcessID;
        processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
        CloseHandle(snapshot);
        if (processHandle == NULL) {
          errorCode = OPEN_PROCESS_FAILED;
          return false;
        }
        return processHandle != nullptr;
      }
    } while (Process32Next(snapshot, &entry));
  }
  CloseHandle(snapshot);
  errorCode = PROCESS_NOT_FOUND;
  return false;
};

std::vector<uintptr_t> MemoryScanner::scanForValue(int targetValue) {
  std::vector<uintptr_t> addresses;
  MEMORY_BASIC_INFORMATION mbi;
  uintptr_t address = 0;

  printf("[DEBUG] Starting scan for value %d\n", targetValue);
  int regionCount = 0;
  int scannedRegions = 0;

  while (VirtualQueryEx(processHandle, reinterpret_cast<LPVOID>(address), &mbi,
                        sizeof(mbi))) {
    regionCount++;

    if (mbi.State == MEM_COMMIT &&
        (mbi.Protect & PAGE_READWRITE || mbi.Protect & PAGE_READONLY)) {

      scannedRegions++;
      // printf("[DEBUG] Scanning region %d: 0x%llx - 0x%llx (size: %zu)\n",
      //        scannedRegions, (uintptr_t)mbi.BaseAddress,
      //        (uintptr_t)mbi.BaseAddress + mbi.RegionSize, mbi.RegionSize);

      std::vector<char> buffer(mbi.RegionSize);
      SIZE_T bytesRead;

      if (ReadProcessMemory(processHandle, mbi.BaseAddress, buffer.data(),
                            mbi.RegionSize, &bytesRead)) {

        for (size_t i = 0; i <= bytesRead - sizeof(int); i += sizeof(int)) {
          int *value = reinterpret_cast<int *>(&buffer[i]);
          if (*value == targetValue) {
            uintptr_t foundAddress =
                reinterpret_cast<uintptr_t>(mbi.BaseAddress) + i;
            // printf("[DEBUG] Found value %d at address 0x%llx\n", targetValue,
            // foundAddress);
            addresses.push_back(foundAddress);
          }
        }
      } else {
        printf("[DEBUG] Failed to read region at 0x%llx\n",
               (uintptr_t)mbi.BaseAddress);
      }
    }
    address += mbi.RegionSize;
  }

  printf("[DEBUG] Scan complete. Checked %d total regions, scanned %d readable "
         "regions\n",
         regionCount, scannedRegions);
  return addresses;
};

void MemoryScanner::Rescan(int value, std::vector<uintptr_t> &addresses_vec) {
  int addr_value;
  for (int i = addresses_vec.size() - 1; i >= 0; i--) {
    bool result = readMemory(addresses_vec[i], addr_value);
    if (result) {
      if (addr_value == value) {
        printf("[+] Value at 0x%llx : %i\n", addresses_vec[i], addr_value);
      } else {
        addresses_vec.erase(addresses_vec.begin() + i);
      }
    }
  }
  printf("[+] Found %zu addresses\n", addresses_vec.size());

  for (size_t i = 0; i < addresses_vec.size(); i++) {
    printf("\t[*] Address found: 0x%llx\n", addresses_vec[i]);
  }
}
