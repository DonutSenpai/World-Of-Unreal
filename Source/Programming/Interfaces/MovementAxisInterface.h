#pragma once

UENUM()
enum class EInputAxis
{
	Forward,
	Right
};

class FMovementAxisInterface
{
public:
	virtual void MovementInputOnAxis( EInputAxis InputOnAxis, float AxisValue ) = 0;

};