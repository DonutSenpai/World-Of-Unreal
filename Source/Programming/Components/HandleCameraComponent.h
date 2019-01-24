#pragma once

#include "Components/SceneComponent.h"
#include "Interfaces/MouseStateInterface.h"
#include "Interfaces/MovementAxisInterface.h"

#include "HandleCameraComponent.generated.h"

UCLASS()
class UHandleCameraComponent : public USceneComponent, public FMouseStateInterface, public FMovementAxisInterface
{
	GENERATED_BODY()

//Uproperty publics
public:
	UHandleCameraComponent();
	// UHandleCameraComponent(class APlayerCharacter* CharacterAttachedTo);

	UPROPERTY(BlueprintReadWrite)
		float CameraZoomSpeed = 150.f;

	void BeginPlay() override;
//code publics
public:

	void NewMouseState(EMouseState NewState);

	void MovementInputRightAxis( float AxisValue );
	void MovementInputForwardAxis( float AxisValue );
	bool bInputOnRightAxis = false;
	bool bInputOnForwardAxis = false;

	void GetControllerAngleDifference( bool AngleRightOrUp /*= true*/ );


	//void NewMouseState_Implementation(EMouseState NewState);

	void RotateCameraFromInput();
	FTimerHandle RotateCameraFromInputHandle;

	void AddCameraZoomInput(float Value);

	void SetOwningCharacter(class APlayerCharacter* CharacterAttachedTo);

	void HandleCameraZoom(float Input);

private:
	void LerpCameraRotation(FRotator TargetRotation);

	EMouseState CurrentMS = EMouseState::None;

	//The character that's owning this component
	class APlayerCharacter* OwnChar;
	
	//Camera zoom internal variables and functions
	void CameraZoomLerp();
	FTimerHandle CameraZoomLerpHandle;
	float CameraBoomTargetLength = 0.f;
	float CameraBoomMaxLength = 900.f;
	float NewCameraBoomLength = 0.f;


};