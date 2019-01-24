#pragma once

#include "MouseStateEnum.h"


class FMouseStateInterface
{
public:
	virtual void NewMouseState(EMouseState NewState) = 0;

	EMouseState CurrentMS = EMouseState::None;
};