#pragma once

struct MoveInputState {
	bool mSneakDown;
	bool mSneakToggleDown;
	bool mWantDownSlow;
	bool mWantUpSlow;
	bool mBlockSelectDown;
	bool mAscendBlock;
	bool mDescendBlock;
	bool mJumpDown;
	bool mSprintDown;
	bool mUpLeft;
	bool mUpRight;
	bool mDownLeft;
	bool mDownRight;
	bool mUp;
	bool mDown;
	bool mLeft;
	bool mRight;
	bool mAscend;
	bool mDescend;
	bool mChangeHeight;
	glm::vec2 mAnalogMoveVector;
	bool mLookCenter;
	unsigned char mLookSlightDirField;
	unsigned char mLookNormalDirField;
	unsigned char mLookSmoothDirField;
	bool mSneakInputCurrentlyDown;
	bool mSneakInputWasReleased;
	bool mSneakInputWasPressed;
	bool mJumpInputWasReleased;
	bool mJumpInputWasPressed;
	bool mJumpInputCurrentlyDown;
};

struct MoveInputComponent {
	MoveInputState mInputState;
	MoveInputState mRawInputState;
	std::chrono::steady_clock::time_point mHoldAutojumpInWaterUtil;
	glm::vec2 mMove;
	glm::vec2 mLookDelta;
	glm::vec2 mInteractDir;
	glm::vec3 mDisplacement;
	glm::vec3 mDisplacementDelta;
	glm::vec3 mCameraOrientation;
	bool mSneaking;
	bool mSprinting;
	bool mWantUp;
	bool mWantDown;
	bool mJumping;
	bool mAutoJumpingInWater;
	bool mMoveInputStateLocked;
	bool mPersistSneak;
	bool mAutoJumpEnabled;
	bool mIsCameraRelativeMovementEnabled;
	bool mIsRotControlledByMoveDirection;
	std::array<bool, 2> mIsPaddling;

	bool isPressed() {
		return mInputState.mUp || mInputState.mDown || mInputState.mRight || mInputState.mLeft;
	}
};
