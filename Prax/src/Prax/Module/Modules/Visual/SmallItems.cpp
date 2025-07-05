//
// Created by vastrakai on 4/19/2024.
//

#include "SmallItems.h"

#include <ext/matrix_transform.hpp>

void SmallItems::OnRenderMatrix(glm::mat4 *matrix) {
    *matrix = glm::translate(*matrix, glm::vec3(50.f / 100, -20.f / 100, -60.f / 100));
}
