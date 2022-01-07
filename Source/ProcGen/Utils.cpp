#include "Utils.h"

#include <cstdio>

namespace Merc {
Color fromHex(unsigned int hexVal) {
  return Color(((hexVal >> 16) & 0xFF) / 255.0, ((hexVal >> 8) & 0xFF) / 255.0,
               ((hexVal)&0xFF) / 255.0);
}

Color fromHex(const char *str) {
  int a, r, g, b;
  sscanf(str + 1, "%02x%02x%02x%02x", &a, &r, &g, &b);
  return Color(r, g, b, a);
}
}  // namespace Merc
