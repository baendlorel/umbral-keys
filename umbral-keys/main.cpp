#include "config.h"
#include "utils.h"
#include "I18N.h"
#include "Logger.h"
#include "UmbralKey.h"

void initialize() {
  I18N::Initialize();
  unordered_map<WORD, Array<WORD>> umbralConfig = LoadConfig();
  //UmbralKey::Add("capslock", Array<chars>{"ctrl", "space"});
  UmbralKey::ApplyConfig(umbralConfig);
  UmbralKey::start();
}

int main() {
  initialize();
  return 0;
}
