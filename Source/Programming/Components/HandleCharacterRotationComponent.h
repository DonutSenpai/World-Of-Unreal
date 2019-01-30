#pragma once

#include "Interfaces/MouseStateInterface.h"
#include "Interfaces/MovementAxisInterface.h"
#include "Components/ActorComponent.h"
#include "HandleCharacterRotationComponent.generated.h"

UCLASS()
class UHandleCharacterRotationComponent : public UActorComponent, public FMouseStateInterface, public FMovementAxisInterface
{
	GENERATED_BODY()

	UHandleCharacterRotationComponent();


	//Owning PlayerCharacter
	class APlayerCharacter* OwnChar = nullptr;

	void BeginPlay() override;

	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction) override;

protected:
	float GetControllerRightAngleDifference();
	float GetControllerUpAngleDifference();

	//"Interface" functions
public:

	void NewMouseState( EMouseState NewState );

	void MovementInputRightAxis( float AxisValue );
	void MovementInputForwardAxis( float AxisValue );

protected:
	UFUNCTION()
	void HandleRotation();

	bool bRotateToCamera = false;
	float LerpAlpha = 0.f;
	float LerpSpeed = 45.f;
	FTimerHandle RotationHandle;


protected:


	void SetCharacterRotation();

	//Rotate X degrees
	void RotateDegrees(float Degrees);
	//Rotate to keep X degrees difference from controller rotation
	void RotateKeepDegrees(float Degrees);

	UFUNCTION()
	void LerpSetRotation(FRotator TargetRotation);


};