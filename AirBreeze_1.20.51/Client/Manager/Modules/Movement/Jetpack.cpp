#include "Jetpack.h"
#include "../../Manager.h"
#include "../../Client.h"
#include "../../Category.h"
#include "../../../SDK/Minecraft.h"
#include "../../../Include/imgui/imgui.h"
#include "../../Module.h"
#include <cmath>

const float M_PI = 3.14159265358979323846; //I don't think we need this long PI, just get 3,14 lol

void Jetpack::onTick() { 
  auto i = Minecraft::getClientInstance(); 
  if (i == nullptr) return; 
  
  auto p = i->LocalPlayer; 
  if (p == nullptr) return; 
  
  auto hr = p->getMovementProxy()->getRot();
  float fs = 1.f;  
  auto pitch = hr.x; 
  auto yaw = hr.y; 
  float ms = 20.0f;
  
  Vec3<float> m(
    -std::sin(yaw * static_cast<float>(M_PI / 180.0)) * std::cos(pitch * static_cast<float>(M_PI / 180.0)) * speed, 
    glide, 
    std::cos(yaw * static_cast<float>(M_PI / 180.0)) * std::cos(pitch * static_cast<float>(M_PI / 180.0)) * speed
  ); 
  
  p->lerpMotion(m); 
}