#pragma once

#include "Interfaces/MouseStateInterface.h"
#include "Interfaces/MovementAxisInterface.h"
#include "Components/ActorComponent.h"
#include "HandleCharacterRotationComponent.generated.h"

UCLASS()
class UHandleCharacterRotationComponent : public UActorComponent, public FMouseStateInterface, public FMovementAxisInterface
{
	GENERATED_BODY()

	//Owning PlayerCharacter
	class APlayerCharacter* OwnChar = nullptr;

	void BeginPlay() override;


	//"Interface" functions
public:

	void NewMouseState( EMouseState NewState );

	void MovementInputRightAxis( float AxisValue );
	void MovementInputForwardAxis( float AxisValue );

protected:

	float GetControllerRightAngleDifference();
	float GetControllerUpAngleDifference();

	void SetCharacterRotation();
	FTimerHandle RotationHandle;

	void LerpCharacterRotation(FRotator TargetRotation);
	void LerpToCameraRotation();
	float LerpSpeed = 45.f;

};