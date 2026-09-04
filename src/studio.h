#ifndef MATRIX_H
#define MATRIX_H

#include <cstdlib>
#include <array>
#include <vector>
#include "Arduino_LED_Matrix.h"

extern std::vector<std::array<uint32_t, 3>> frames;

std::array<uint32_t, 3> arduinoStudio(const String& update);

#endif