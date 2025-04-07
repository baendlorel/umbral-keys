﻿#include "config-loader.h"
#include "i18n.h"
#include "logger.h"
#include "umbral-key.h"
#include "utils.h"

void initialize() {
  I18N::Initialize();
  unordered_map<WORD, Array<WORD>> umbralConfig = LoadConfig();
  //UmbralKey::Add("capslock", Array<chars>{"ctrl", "space"});
  UmbralKey::ApplyConfig(umbralConfig);
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
