#pragma once

enum JumpType : int32_t {
	NONE   = 0x0000,
	HOP    = 0x0001,
	STEP   = 0x0002,
	SPRINT = 0x0003,
	_COUNT = 0x0004,
};