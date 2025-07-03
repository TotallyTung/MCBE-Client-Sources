#pragma once

class LevelRendererPlayer {
public:
	BUILD_ACCESS(this, Vector3<float>, origin, 0x5E4);
};
class LevelRenderer {
public:
	BUILD_ACCESS(this, LevelRendererPlayer*, levelRendererPlayer, 0x308);
};