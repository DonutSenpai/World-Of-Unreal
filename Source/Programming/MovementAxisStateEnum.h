#pragma once

#include "MovementAxisStateEnum.generated.h"

UENUM(BlueprintType)
enum class EMovementAxisState : uint8
{
	None,
	Forward,
	Right,
	ForwardRight

};