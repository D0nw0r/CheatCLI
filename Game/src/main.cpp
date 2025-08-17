#include <cstdio>
#include <iostream>
#include <limits>
#include <windows.h>

struct Weapon {
  int id;
  int ammo;
};

struct Player {
  int id;
  int health;
  int group;
  Weapon weapon;
};

void CreatePlayer(Player &player) {
  player.health = 500;
  player.group = 1;
  player.id = 1;
  player.weapon.id = 1;
  player.weapon.ammo = 20;
}

int main(int argc, char *argv[]) {
  Player player1;
  CreatePlayer(player1);
  // Print the exact address
  printf("[GAME] Player health (500) is at address: %p\n",
         (void *)&player1.health);

  printf("[GAME] Player struct is at: %p\n", (void *)&player1);
  printf("[GAME] Health value: %d\n", player1.health);
  printf("[GAME] Process ID: %lu\n", GetCurrentProcessId());
  printf("Ready for attachment (press Enter)\n");
  getchar();

  while (true) {
    printf(" --- Menu ---\n");
    printf("[1] Take damage (-10hp)\n");
    printf("[2] Show Hp\n");
    printf("[3] Reset Hp\n");
    int choice;
    if (!(std::cin >> choice)) { // Only read once!
      printf("[!] Invalid input! Please enter a number.\n");
      std::cin.clear();
      std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
      continue;
    }
    switch (choice) {
    case 1:
      player1.health -= 10;
      printf("[GAME] Player health (%i) is at address: %p\n", player1.health,
             &player1.health);

      break;
    case 2:
      printf("[GAME] Player health (%i) is at address: %p\n", player1.health,
             &player1.health);
      break;
    case 3:
      player1.health = 500;
      printf("[+] Hp reset to: %i\n", player1.health);
      printf("[GAME] Player health (%i) is at address: %p\n", player1.health,
             &player1.health);
      break;

    default:
      printf("[!] Invalid option\n");
      break;
    }
  }

  return 0;
}
