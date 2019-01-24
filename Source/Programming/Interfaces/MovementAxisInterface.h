#pragma once


class FMovementAxisInterface
{
public:

	virtual void MovementInputRightAxis( float AxisValue ) = 0;
	virtual void MovementInputForwardAxis( float AxisValue ) = 0;

};