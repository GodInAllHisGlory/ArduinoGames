#include <cstdlib>
#include <vector>
#include <array>
#include "Arduino_LED_Matrix.h"

std::vector<std::array<uint32_t, 3>> frames = {};
//Parser and animation updater
//Get frame number
//Parse out uint32
//Animation frames update/add frame number uint32
//Reset animation cycle
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

//Animation cycle
//while(true)
//Matrix update with animation frames[index]
//Delay for some amount of time
//If index = global frames length set to 0 otherwise add 1

