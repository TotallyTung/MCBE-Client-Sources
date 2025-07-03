#pragma once

class Weather {
public:
    CLASS_FIELD(float, fogLevel, 0x4C); // 1.21.50
    CLASS_FIELD(float, lightingLevel, 0x44); // 1.21.50
    CLASS_FIELD(float, rainLevel, 0x38); // 1.21.50
};