#pragma once

#include "MovementAxisStateEnum.h"

class FMovementAxisInterface
{
public:

	virtual void MovementInputRightAxis( float AxisValue ) = 0;
	virtual void MovementInputForwardAxis( float AxisValue ) = 0;

	EMovementAxisState CurrentMAS = EMovementAxisState::None;

	bool bInputOnRightAxis = false;
	bool bInputOnForwardAxis = false;

};