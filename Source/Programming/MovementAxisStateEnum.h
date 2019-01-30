#pragma once

#include "MovementAxisStateEnum.generated.h"

UENUM()
enum class EMovementAxisState : uint8
{
	None,
	Forward,
	Right,
	ForwardRight

};