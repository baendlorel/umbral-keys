#include "config-loader.h"
#include "i18n.h"
#include "logger.h"
#include "umbral-key.h"
#include "utils.h"

void initialize() {
  I18N::Initialize();
  unordered_map<string, Array<string>> umbralConfig = LoadConfig();
  UmbralKey::add("capslock", Array<chars>{"ctrl", "space"});
  UmbralKey::start();
}

int main() {
  initialize();

  Array<string> umbras = split("ctrl++space+", '+');
  cout << "size: " << umbras.getSize() << endl;
  for (size_t i = 0; i < umbras.getSize(); i++) {
    cout << i << " -> " << umbras.getSize() << endl;
    cout << "[" << umbras[i] << "]" << endl;
  }

  return 0;
}
