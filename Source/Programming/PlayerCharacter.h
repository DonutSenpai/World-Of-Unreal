#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MouseStateEnum.h"
#include "Interfaces/MouseStateInterface.h"
#include "PlayerCharacter.generated.h"


UCLASS()
class APlayerCharacter : public ACharacter, public FMouseStateInterface
{
	GENERATED_BODY()


	UPROPERTY(VisibleAnywhere)
	class UHandleCameraComponent* HandleCameraComp;

	UPROPERTY(VisibleAnywhere)
	class UHandleCharacterRotationComponent* CharacterRotationComp;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

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
	void HandleLookVertical(float Value);
	void HandleLookHorizontal(float Value);

	void HandleJump();

	UFUNCTION()
	void NewMouseState(EMouseState NewState);

	UFUNCTION()
	void HandleCharacterRotation();
	void SetCharacterRotationInternal(FRotator RotationToSet, float LerpSpeed = 0.f);	

/*
	UFUNCTION()
	void HandleCameraRotation();*/

private:
	
	//Vars for handling of the Characters rotation
	float MaxControlRotationDifference = 45.f;
	float CharacterRotationDirection = 0.f;
	float DegreesToRotate = 0.f;
	FVector VectorToRotateTo = FVector();
	float RotationLerpSpeed = 0.f;
	
	FTimerHandle CharacterRotationHandle;
	FTimerHandle CameraRotationhandle;

	bool bAutoRun = false;

	//Just camera boom and zoom things
	void HandleCameraZoom(float InputValue);
	void ZoomLerp();


	
public:
	class AMainPlayerController *PlayerController;

};