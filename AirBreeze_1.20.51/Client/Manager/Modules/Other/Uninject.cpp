#include "Uninject.h"
#include "../../Manager.h"

void UninjectModule::onEnable() {
	Client* client = nullptr;
	Manager manager(client);
	manager.uninjectDLL();
}
