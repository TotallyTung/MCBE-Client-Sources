#pragma once
#include "../../ModuleBase/Module.h"

class ViewModel : public Module {
public:
	int mainHand = 0;
	bool reset = false;
	bool translate = false;
	bool scale = false;
	bool rotate = false;
	Vec3<float> translateVec;
	Vec3<float> scaleVec = Vec3<float>(1.f, 1.f, 1.f);
	Vec3<float> rotateVec;
	float rotAngle = 0.1f;
	ViewModel();
	void onMatrixRender(glm::mat4* matrix) override;
};