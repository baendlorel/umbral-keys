#include "umbral-key.h"

int main() {
  UmbralKey::add("CapsLock", VK_CAPITAL, new WORD[2]{VK_CONTROL, VK_SPACE}, 2);
  UmbralKey::start();
  return 0;
}
