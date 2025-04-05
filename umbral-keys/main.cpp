#include "umbral-key.h"

int main() {
  UmbralKey::add("CapsLock", "capslock", new const char*[2]{"ctrl", "space"},
                 2);
  UmbralKey::start();
  return 0;
}
