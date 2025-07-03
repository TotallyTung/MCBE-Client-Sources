#include "CoordsCommand.h"

CoordsCommand::CoordsCommand() : IMCCommand("coords", "Prints your coordinates", "") {
	registerAlias("pos");
	registerAlias("position");
}

CoordsCommand::~CoordsCommand() {
}

bool CoordsCommand::execute(std::vector<std::string>* args) {
	Vec3* pos = Game.getLocalPlayer()->getPos();
	float yPos = pos->y - 1.62f;
	clientMessageF(u8"%s %sX: %.2f Y: %.2f Z: %.2f", Utils::getCommandClientName(), GREEN, pos->x, yPos /* eye height */, pos->z);
	return true;
}
