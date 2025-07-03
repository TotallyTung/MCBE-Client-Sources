#pragma once
#include "../Module.h"
#include "../../ModuleManager.h"

class NoClipGlide : public IModule {
	public: 
		float glide = -0.01f;
	    float glideModEffective = 0;

	public:
	NoClipGlide();
	~NoClipGlide();

	virtual const char* getRawModuleName();
};
