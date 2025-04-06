#include "config-loader.h"
#include "i18n.h"
#include "logger.h"
#include "umbral-key.h"
#include "utils.h"

void initialize() {
  I18N::Initialize();
  UmbralKey::add("capslock", new const char*[2]{"ctrl", "space"}, 2);
  UmbralKey::start();
}

int main() {
  initialize();

  NativeArray<string> umbras = split("ctrl++space+", '+');
  cout << "size: " << umbras.size << endl;
  for (size_t i = 0; i < umbras.size; i++) {
    cout << i << " -> " << umbras.size << endl;
    cout << "[" << umbras.data[i] << "]" << endl;
  }

  return 0;
}
