#include "scanner.h"
#include <Windows.h>
#include <basetsd.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <errhandlingapi.h>
#include <iostream>
#include <memoryapi.h>
#include <string>
#include <vector>

void ReScanLoop(MemoryScanner *mem_scanner, int value_to_scan,
                std::vector<uintptr_t> &addresses_vec) {
  while (true) {
    printf("[?] Rescan? (y/n)\n");
    std::string choice;
    std::cin >> choice;
    if (choice == "y" || choice == "Y") {
      printf("[?] Value to rescan the known addresses: ");
      std::cin >> value_to_scan;
      mem_scanner->Rescan(value_to_scan, addresses_vec);
      continue;
    }
    if (choice == "n" || choice == "") {
      printf("[!] Exiting scan mode.\n");
      break;
    } else {
      printf("[!] Invalid choice (y|n)\n");
      continue;
    }
  }

  return;
}

int main(int argc, char *argv[]) {

  if (argc < 2) {
    printf("[!] Usage: CheatCli.exe <Game Process Name>");
    return -1;
  }

  std::string game_process_name = argv[1];

  MemoryScanner *mem_scanner = new MemoryScanner();
  if (!mem_scanner->attachToProcess(game_process_name)) {

    if (mem_scanner->getErrorCode() != 0) {
      printf("Attaching failed with error code %i\n",
             mem_scanner->getErrorCode());
      return -1;
    }
  }
  printf("[+] Successfully attached to the Game process with PID %lu\n",
         mem_scanner->getProcId());
  int value_to_scan;
  printf("[?] Enter the value to be scanned: ");
  std::cin >> value_to_scan;

  printf("[+] Scanning Process %lu memory for the value %i\n",
         mem_scanner->getProcId(), value_to_scan);
  std::vector<uintptr_t> addresses_vec =
      mem_scanner->scanForValue(value_to_scan);
  printf("[+] Found %zu addresses\n", addresses_vec.size());

  for (size_t i = 0; i < addresses_vec.size(); i++) {
    printf("\t[*] Address found: 0x%llx\n", addresses_vec[i]);
  }

  ReScanLoop(mem_scanner, value_to_scan, addresses_vec);

  printf("[+] Select which address to write to:\n");
  for (int i = 0; i < addresses_vec.size(); i++) {
    printf("\t[*][%i] : 0x%llx\n", i, addresses_vec[i]);
  }

  std::cin >> value_to_scan;
  uintptr_t selected_addr = addresses_vec[value_to_scan];

  int value_to_write;
  SIZE_T numberOfBytesWritten;
  printf("[?] Which value to write?\n");
  std::cin >> value_to_write;

  if (!WriteProcessMemory(mem_scanner->getProcessHandle(),
                          reinterpret_cast<LPVOID>(selected_addr),
                          reinterpret_cast<LPCVOID>(&value_to_write),
                          sizeof(int), &numberOfBytesWritten)) {
    printf("[!] Failed to write to Process Memory: %lu\n", GetLastError());
  }

  printf("Written %llu bytes to 0x%llx with value %i\n", numberOfBytesWritten,
         selected_addr, value_to_write);

  getchar();

  return 0;
}
