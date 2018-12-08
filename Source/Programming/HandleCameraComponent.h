#pragma once

#include "Components/SceneComponent.h"
#include "HandleCameraComponent.generated.h"

UCLASS()
class UHandleCameraComponent : public USceneComponent
{
	GENERATED_BODY()

//Uproperty publics
public:
	UHandleCameraComponent();
	// UHandleCameraComponent(class APlayerCharacter* CharacterAttachedTo);

	UPROPERTY(BlueprintReadWrite)
		float CameraZoomSpeed = 150.f;

//code publics
public:

	void AddCameraPitchInput(float Value);
	void AddCameraYawInput(float Value);

	void AddCameraZoomInput(float Value);

	void SetOwningCharacter(class APlayerCharacter* CharacterAttachedTo);
	void SetTargetCameraBoom(class USpringArmComponent* Boom);

	void HandleCameraZoom(float Input);

private:
	//The character that's owning this component
	class APlayerCharacter* OwnChar;
	class USpringArmComponent* TargetBoom;
	
	//Camera zoom internal variables and functions
	void CameraZoomLerp();
	FTimerHandle CameraZoomLerpHandle;
	float CameraBoomTargetLength = 0.f;
	float CameraBoomMaxLength = 900.f;
	float NewCameraBoomLength = 0.f;

	void BeginPlay() override;

};