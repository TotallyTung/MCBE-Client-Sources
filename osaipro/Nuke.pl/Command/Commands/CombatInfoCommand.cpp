#include "CombatInfoCommand.h"
#include "../../Config/ConfigManager.h"
#include "../../Module/ModuleManager.h"

CombatInfoCommand::CombatInfoCommand() : IMCCommand("CombatInfo", "CombatInfos modules <add/remove<->delete>", "<add/remove<->delete> <ModName/Alias> <Alias>") {
	registerAlias("combati");
	registerAlias("cinfo");
	registerAlias("ci");
}

CombatInfoCommand::~CombatInfoCommand() {
}

bool CombatInfoCommand::execute(std::vector<std::string>* args) {
	if (args->size() <= 2) return false;
	std::string argName = args->at(1);
	static CombatInfo* ci = moduleMgr->getModule<CombatInfo>();
	if (argName == "add") {
		std::string moduleName = args->at(2);
		std::string AliasName = args->at(3);
		bool found = false;
		if (moduleName.size() > 0 && AliasName.size() > 0) {
			if (!moduleMgr->getModuleByName(moduleName).has_value()) {
				clientMessageF("%s%s%s%s is Not Found!", Utils::getCommandClientName(), GREEN, moduleName, RED);
				return true;
			};
			for (std::pair<std::string, std::string> mn_nmal : ci->ModName_NameAlias) {
				if (mn_nmal.first == moduleMgr->getModuleByName(moduleName).value()->getRawModuleName() || mn_nmal.second == AliasName)
					found = true;
			};
			if (!found) {
				ci->ModName_NameAlias.push_back(std::make_pair(moduleMgr->getModuleByName(moduleName).value()->getRawModuleName(), args->at(3)));
				clientMessageF("%s%sSuccessfully Added %s%s(%s%s%s)%s!", Utils::getCommandClientName(), GREEN, AliasName, WHITE, GREEN, moduleMgr->getModuleByName(moduleName).value()->getRawModuleName(), WHITE, RED);
			}
			else
				clientMessageF("%s%s%s%s(%s%s%s)%s is Already Added!", Utils::getCommandClientName(), RED, AliasName, WHITE, GREEN, moduleMgr->getModuleByName(moduleName).value()->getRawModuleName(), WHITE, RED);
			return true;
		}
	}
	else if (argName == "remove") {
		std::string moduleName = args->at(2);
		if (moduleName.size() > 0) {
			int i = 0;
			for (std::pair<std::string, std::string> mn_nmal : ci->ModName_NameAlias) {
				if (moduleName == mn_nmal.first || moduleName == mn_nmal.second) {
					clientMessageF("%s%sSuccessfully Removed %s%s%s(%s%s%s)%s!", Utils::getCommandClientName(), RED, GREEN, moduleMgr->getModuleByName(moduleName).value()->getRawModuleName(), WHITE, GREEN, mn_nmal.second, WHITE, RED);
					ci->ModName_NameAlias.erase(ci->ModName_NameAlias.begin() + i);
					return true;
				};
				i += 1;
			}
			clientMessageF("%s%s%s%s is Not Found!", Utils::getCommandClientName(), GREEN, moduleName, RED);
			return true;
		}
	}
	else if (argName == "clear") {
		clientMessageF("%s%sSuccessfully Cleared CombatInfo Lists!", Utils::getCommandClientName(), GREEN);
		ci->ModName_NameAlias.clear();
	}
	else {
		return false;
	};
	return true;
}
