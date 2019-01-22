#pragma once

#include "MouseStateEnum.generated.h"


UENUM()
enum class EMouseState : uint8
{
	None,
	LeftHeld,
	RightHeld,
	BothHeld

};