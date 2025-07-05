#pragma once
#include "ICommand.h"
#include <sstream>

#ifdef _BETA
class PasswordCommand : public IMCCommand {
public:
								// 1               2         3           4         5              6          7         8            9          10         11           12          13           14        15           16         17            18          19         20            21          22         23           24         25           26         27         28           29        30          31           32
	const uint32_t matchPW[33] = {3032770428, 3274002126, 163086608, 4230224777, 2039851928, 3675653500, 3328843349, 3328843349, 3885879077, 958491549, 3061013261, 1792958942, 3143304174, 3143304174, 3143304174, 3143304174, 3143304174, 1916298011, 1916298011, 1275806891, 3306539612, 3306539612, 1446547125, 1916298011, 1916298011, 1916298011, 1916298011, 1916298011, 3438163959, 228656751, 1916298011, 1916298011};

	int tries = 3;
	bool hasChecked = false;

	PasswordCommand();
	~PasswordCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};
#endif
