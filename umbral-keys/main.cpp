#include <Windows.h>
#include <iostream>
#include "umbral-key.h"

int main() {
  std::cout << "Program running, press CapsLock to test behavior..."
            << std::endl;

  UmbralKey::add("CapsLock", VK_CAPITAL, new WORD[2]{VK_CONTROL, VK_SPACE}, 2);
 
  UmbralKey::start();
  return 0;
}
