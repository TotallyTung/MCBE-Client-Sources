#include "FakePlayer.h"
FakePlayer::FakePlayer() : IModule(0, Category::MISC, "FakePlayer") {
}
FakePlayer::~FakePlayer() {
};
const char* FakePlayer::getModuleName() {
	return "FakePlayer";
}
bool SpawnEntity = false;
void FakePlayer::onEnable() {
	FakePlayerPos = Game.getLocalPlayer()->getPos()->floor();
	SpawnEntity = true;
}
void FakePlayer::onPostRender(MinecraftUIRenderContext* renderCtx) {
}
void FakePlayer::onDisable() {
	FakePlayerPos = Vec3(0, 0, 0);
	SpawnEntity = false;
}