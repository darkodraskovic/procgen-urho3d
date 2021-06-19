#ifndef UTILS_H
#define UTILS_H

#include <Urho3D/Engine/Application.h>
#include <Urho3D/Math/Color.h>

using namespace Urho3D;

namespace Merc {
    Color fromHex(unsigned int hexVal);
    Color fromHex(const char *str);
}

#endif /* UTILS_H */
