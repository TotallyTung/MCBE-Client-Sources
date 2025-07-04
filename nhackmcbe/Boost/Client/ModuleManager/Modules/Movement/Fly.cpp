#include "Fly.h"

Fly::Fly() : Module("VeloFly", "Velocityfly", Category::MOVEMENT) {
	addSlider<float>("Horizontal Speed", "NULL", ValueType::FLOAT_T, &speed, 0.03, 10.f);
	addSlider<float>("Vertical Speed", "NULL", ValueType::FLOAT_T, &this->upanddown, 0.03, 10.f);
	this->addBoolCheck("Save Old POS", "NULL", &this->NoPacket);
	addSlider<float>("Glide", "How fast you want to glide.", ValueType::FLOAT_T, &glideMod, -0.2f, 0.f);
	addBoolCheck("SM Border", "NULL", &smborder);

}

Fly::~Fly() {
}

void Fly::onNormalTick(Actor* actor) {
	auto gm = mc.getGameMode();
	gm->player->stateVectorComponent->velocity = Vec3<float>(0, 0, 0);
	glideModEffective = glideMod;
	if (mc.canUseMoveKeys()) {
		if (mc.isKeyDown(VK_SPACE))
			glideModEffective += upanddown;
		if (mc.isKeyDown(VK_SHIFT))
			glideModEffective -= upanddown;
	}
	if (smborder) {
		auto* pos = mc.getLocalPlayer()->getPosition();

		if (pos->x > 1090)
			mc.getLocalPlayer()->setPos(Vec3(pos->x = 1090, pos->y, pos->z));

		if (pos->z > 1090)
			mc.getLocalPlayer()->setPos(Vec3(pos->x, pos->y, pos->z = 1090));

		if (pos->x < 60)
			mc.getLocalPlayer()->setPos(Vec3(pos->x = 60, pos->y, pos->z));

		if (pos->z < 60)
			mc.getLocalPlayer()->setPos(Vec3(pos->x, pos->y, pos->z = 60));
		if (pos->y > 155)
			mc.getLocalPlayer()->setPos(Vec3(pos->x, pos->y = 155, pos->z));
	};
	gm->player->stateVectorComponent->velocity.y = glideModEffective;
	auto player = mc.getLocalPlayer();
	if (player == nullptr) return;
	bool w = mc.isKeyDown('W');
	bool a = mc.isKeyDown('A');
	bool s = mc.isKeyDown('S');
	bool d = mc.isKeyDown('D');

	float yaw = player->rotationComponent->Get().y;

	bool pressed = w || a || s || d;

	if (w && s)
		return;


	if (d) {
		yaw += 90.f;
		if (w)
			yaw -= 45.f;
		else if (s)
			yaw += 45.f;
	}
	if (a) {
		yaw -= 90.f;
		if (w)
			yaw += 45.f;
		else if (s)
			yaw -= 45.f;
	}

	if (s && !a && !d)
		yaw += 180.f;

	float calcYaw = (yaw + 90) * (PI / 180);
	Vec3<float> moveVec;
	moveVec.x = cos(calcYaw) * speed;
	moveVec.y = player->stateVectorComponent->velocity.y;
	moveVec.z = sin(calcYaw) * speed;
	if (pressed) player->lerpMotion(moveVec);
}

void Fly::onEnable() {
}

void Fly::onDisable() {
	if (mc.getLocalPlayer() != nullptr)return;
}

