#pragma once

#include "Interfaces/MouseStateInterface.h"
#include "Interfaces/MovementAxisInterface.h"
#include "Components/ActorComponent.h"
#include "HandleCharacterRotationComponent.generated.h"

UENUM(BlueprintType)
enum class ECharacterRotationAction : uint8
{
	E_None,
	E_AddRotation,
	E_RotateToCamera,
	E_RotateAtAngle,
	E_RotateToAngle45,
	E_RotateToAngle90,
	E_InheritControlRotation,
};


UCLASS()
class UHandleCharacterRotationComponent : public UActorComponent, public FMouseStateInterface, public FMovementAxisInterface
{
	GENERATED_BODY()

	UHandleCharacterRotationComponent();


	//Owning PlayerCharacter
	class APlayerCharacter* OwnChar = nullptr;

	void BeginPlay() override;

	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction) override;

	UFUNCTION()
	void HandleRotation();

public:
	//Input handling
	void RightMovAxisPressed();
	void RightMovAxisReleased();

	void ForwardMovAxisPressed();

	void RightActionButtonReleased();

	void NewMouseState( EMouseState NewState );

	void MovementInputRightAxis( float AxisValue );
	void MovementInputForwardAxis( float AxisValue );

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	ECharacterRotationAction RotationAction = ECharacterRotationAction::E_None;
protected:


	float LerpSpeed = 45.f;
	float PreviousAxisValue = 0.f;

//Functions
protected:

	void RotateToCamera();
	void RotateToAngle(float Degrees);

	//Rotate to keep X degrees difference from controller rotation
	void RotateAtAngle(float Degrees);

	void AddRotation();

	UFUNCTION()
	void LerpSetRotation(FRotator TargetRotation);

	float GetControllerRightAngleDifference();
	float GetControllerUpAngleDifference();

};