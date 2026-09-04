#include <cstdlib>
#include <vector>
#include <array>
#include "Arduino_LED_Matrix.h"

std::vector<std::array<uint32_t, 3>> frames = {{0x0, 0x0, 0x0}};

std::array<uint32_t, 3> matrixParser(const String& uri){
    std::array<uint32_t, 3> parsedFrame = {}; // Holds the values sent from the client.
    String num = "";
    int frameIndex = 0;

    for (unsigned int pointer = 1; pointer < uri.length() && frameIndex < 3; pointer++) {
      const char c = uri.charAt(pointer);
      if (c == '-' || pointer == uri.length() - 1) {
        if (pointer == uri.length() - 1 && c != '-') {
          num.concat(c);
        }
        parsedFrame[frameIndex] = static_cast<uint32_t>(strtoul(num.c_str(), nullptr, 0));
        Serial.println(num);
        num = "";
        frameIndex++;
      } else {
        num.concat(c);
      }
    }
    frames.push_back(parsedFrame);
    return parsedFrame;
}

std::array<uint32_t, 3> arduinoStudio(const String& update){
  if(update == "") return frames[0];

  return matrixParser(update);
}