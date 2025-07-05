//
// Created by vastrakai on 4/19/2024.
//

#include "ViewModel.h"

#include <ext/matrix_transform.hpp>

void ViewModel::OnRenderMatrix(glm::mat4 *matrix) {
    if (Translate.Enabled) *matrix = glm::translate(*matrix, glm::vec3(TranslateX.Value / 100, TranslateY.Value / 100, TranslateZ.Value / 100));
    if (Rotate.Enabled) {
        *matrix = glm::rotate(*matrix, glm::radians(RotateX.Value), glm::vec3(1, 0, 0));
        *matrix = glm::rotate(*matrix, glm::radians(RotateY.Value), glm::vec3(0, 1, 0));
        *matrix = glm::rotate(*matrix, glm::radians(RotateZ.Value), glm::vec3(0, 0, 1));
    }
}
