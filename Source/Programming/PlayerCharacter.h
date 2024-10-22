#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
//#include "HandleCameraComponent.h"
#include "PlayerCharacter.generated.h"


UCLASS()
class APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere)
	class UHandleCameraComponent* HandleCameraComp;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	
	
	APlayerCharacter();

public:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere)
	class UArrowComponent* MyArrow;

	UPROPERTY(VisibleAnywhere)
	class UArrowComponent* MyOtherArrow;

	UPROPERTY(BlueprintReadOnly)
	FVector ControlYaw = FVector();
	
private:
	//just a test method
	UFUNCTION()
	void Thing(bool yes);
	//handles input
	void HandleMoveForward(float Value);
	void HandleMoveRight(float Value);
	UFUNCTION()
	void HandleLookVertical(float Value);
	UFUNCTION()
	void HandleLookHorizontal(float Value);
	void HandleJump();

	UFUNCTION()
	void HandleRightActionButton(bool Pressed);
	UFUNCTION()
	void HandleLeftActionButton(bool Pressed);

	UFUNCTION()
	void HandleCharacterRotation();
	void SetCharacterRotationInternal(FRotator RotationToSet, float LerpSpeed = 0.f);	

	UFUNCTION()
	void HandleCameraRotation();

	//Gets the angle difference in degrees between this actor and the controller
	// @param AngleRightOrUp returns angle right if true and angle up if false
	// @Param bSigned returns a signed angle instead of an absolute value of the angle
	float GetControllerAngleDifference(bool bAngleRightOrUp = true);

private:
	
	//Vars for handling of the Characters rotation
	float MaxControlRotationDifference = 45.f;
	float CharacterRotationDirection = 0.f;
	float DegreesToRotate = 0.f;
	FVector VectorToRotateTo = FVector();
	float RotationLerpSpeed = 0.f;
	
	FTimerHandle CharacterRotationHandle;
	FTimerHandle CameraRotationhandle;

	bool bRotateCamera = false;
	bool bAutoRun = false;

	//Just camera boom and zoom things
	void HandleCameraZoom(float InputValue);
	void ZoomLerp();


	
	class AMainPlayerController *MyPC;


};