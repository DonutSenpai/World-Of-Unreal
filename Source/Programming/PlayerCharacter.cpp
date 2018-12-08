#include "PlayerCharacter.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "MainPlayerController.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HandleCameraComponent.h"


APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	//Set size for collision capsule
	//GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	HandleCameraComp = CreateDefaultSubobject<UHandleCameraComponent>("HandleCameraComponent");
	HandleCameraComp->SetupAttachment(RootComponent);
	HandleCameraComp->SetOwningCharacter(this);


	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(HandleCameraComp);
	HandleCameraComp->SetTargetCameraBoom(CameraBoom);
	
	CameraBoom->TargetArmLength = 650.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = true; // Camera does not rotate relative to arm

	MyArrow = CreateDefaultSubobject<UArrowComponent>("MyArrow");
	MyArrow->SetupAttachment(RootComponent);
	//MyArrow->bHiddenInGame = false;
	MyArrow->ArrowSize = 2.f;

	MyOtherArrow = CreateDefaultSubobject<UArrowComponent>("MyOtherArrow");
	MyOtherArrow->SetupAttachment(RootComponent);
	//MyOtherArrow->bHiddenInGame = false;
	MyOtherArrow->ArrowSize = 2.5f;
	MyOtherArrow->ArrowColor = FColor::Emerald;

}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

/*
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("BeginPlay in character is run"));*/

	MyPC = Cast<AMainPlayerController>(GetWorld()->GetFirstPlayerController());
	MyPC->RightActionButton.AddDynamic(this, &APlayerCharacter::HandleRightActionButton);
	MyPC->LeftActionButton.AddDynamic(this, &APlayerCharacter::HandleLeftActionButton);

}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	HandleCharacterRotation();
	HandleCameraRotation();
}

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::HandleMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::HandleMoveRight);
	PlayerInputComponent->BindAxis("LookVertical", this, &APlayerCharacter::HandleLookVertical);
	PlayerInputComponent->BindAxis("LookHorizontal", this, &APlayerCharacter::HandleLookHorizontal);
	PlayerInputComponent->BindAxis("CameraZoom", this, &APlayerCharacter::HandleCameraZoom);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::HandleJump);

}


void APlayerCharacter::Thing(bool yes)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Purple, FString::Printf(TEXT("%d THING HAPPNEEEEED! YAAAY!"), yes));
	Jump();
}

void APlayerCharacter::HandleMoveForward(float Value)
{
	if (bAutoRun)
		AddMovementInput(GetActorForwardVector(), 5.f);

	else
		AddMovementInput(GetActorForwardVector(), Value);
}

void APlayerCharacter::HandleMoveRight(float Value)
{
	AddMovementInput(GetActorRightVector(), Value);
}

void APlayerCharacter::HandleLookVertical(float Value)
{
	if (bRotateCamera)
		AddControllerPitchInput(-Value);
}

void APlayerCharacter::HandleLookHorizontal(float Value)
{
	if (bRotateCamera)
		AddControllerYawInput(Value);

	if (GEngine && Value != 0)
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Emerald, FString::Printf(TEXT("Input value: %f"), Value));
}

void APlayerCharacter::HandleJump()
{
	Jump();
}

void APlayerCharacter::HandleRightActionButton(bool Pressed)
{

	if (!MyPC->bHoldsLeft)
	{
		bRotateCamera = Pressed;
	}

	//If holds both buttons set autorun to true
	if (MyPC->bHoldsRight && MyPC->bHoldsLeft)
	{
		bAutoRun = true;
	}
	
	else if (!Pressed)
	{
		bAutoRun = false;
	}

}

void APlayerCharacter::HandleLeftActionButton(bool Pressed)
{
	if (!MyPC->bHoldsRight)
	bRotateCamera = Pressed;

	if (MyPC->bHoldsRight && MyPC->bHoldsLeft)
		bAutoRun = true;
	
	else if (!Pressed)
		bAutoRun = false;
}

void APlayerCharacter::HandleCameraRotation()
{
	if (InputComponent->GetAxisValue("MoveForward") != 0 && MyPC->CurrentMS == EMouseState::None && !(InputComponent->GetAxisValue("LookHorizontal") != 0))
	{	
		float AngleRight = GetControllerAngleDifference(true);
		float AngleUp = FMath::Clamp(GetControllerAngleDifference(false), 0.f, 20.f);

		FRotator TargetRotation = GetActorForwardVector().RotateAngleAxis(AngleUp, GetActorRightVector()).ToOrientationRotator();
		
		FRotator NewControlRotation = FMath::RInterpConstantTo(GetControlRotation(), TargetRotation, FApp::GetDeltaTime(), 250.f);

		MyPC->SetControlRotation(NewControlRotation);
	}
}

float APlayerCharacter::GetControllerAngleDifference(bool AngleRightOrUp /*= true*/)
{
	if (AngleRightOrUp)
	{
		FVector ControlYaw = FRotator(0.f, GetControlRotation().Yaw, 0.f).Vector();
		float DotProductYaw = FVector::DotProduct(ControlYaw, GetActorForwardVector());
		float DotProductYawRight = FVector::DotProduct(ControlYaw, GetActorRightVector());
		float AngleRight = FMath::RadiansToDegrees(FMath::Acos(DotProductYaw)) * (DotProductYawRight < 0 ? -1 : 1);

		return AngleRight;
	}

	else
	{
		FVector ControlPitch = FRotator(GetControlRotation().Pitch, 0.f, 0.f).Vector();
		float DotProductPitchUp = FVector::DotProduct(ControlPitch, GetActorUpVector());
		float AngleUp = FMath::RadiansToDegrees(FMath::Acos(DotProductPitchUp)) - 90.f;

		return AngleUp;
	}
}

void APlayerCharacter::HandleCharacterRotation()
{
	//Calculate things for holding down right
	if (MyPC->bHoldsRight)
	{
		//resetting variables
		RotationLerpSpeed = 0.f;
		VectorToRotateTo = FVector(0.f, 0.f, 0.f);

		//Zero out not needed axes and calculate the projections
		ControlYaw = FRotator(0.f, GetControlRotation().Yaw, 0.f).Vector();
		float DotProductRight = FVector::DotProduct(ControlYaw, GetActorRightVector());

		//The calculated angle between this actor and the controller
		float DotProductForward = FVector::DotProduct(ControlYaw, GetActorForwardVector());
		float Angle = FMath::RadiansToDegrees(FMath::Acos(DotProductForward));

		//Calculates if the controller is rotated to the right or to the left and sets the direction var
		if (DotProductRight < 0)
		{
			CharacterRotationDirection = -1.f;
		}
		else if (DotProductRight > 0)
		{
			CharacterRotationDirection = 1.f;
		}

		//Calculating things for moving 
		if (GetCharacterMovement()->Velocity.Size() != 0)
		{
			DegreesToRotate = Angle * CharacterRotationDirection;
			VectorToRotateTo = GetActorForwardVector().RotateAngleAxis(DegreesToRotate, FVector(0.f, 0.f, 1.f));
			RotationLerpSpeed = 0.f;
		}

		if (Angle >= MaxControlRotationDifference && !(GetCharacterMovement()->Velocity.Size()))
		{
			if (InputComponent->GetAxisValue("LookHorizontal") != 0)
			{
				DegreesToRotate = (Angle - MaxControlRotationDifference) * CharacterRotationDirection;
				VectorToRotateTo = GetActorForwardVector().RotateAngleAxis(DegreesToRotate, FVector(0.f, 0.f, 1.f));
				RotationLerpSpeed = 45.f;
			}

		}

		if (VectorToRotateTo.Size() > 0)
		{
			SetCharacterRotationInternal(VectorToRotateTo.Rotation(), RotationLerpSpeed);
		}

	}

}

void APlayerCharacter::SetCharacterRotationInternal(FRotator RotationToSet, float LerpSpeed)
{
	if (LerpSpeed <= 0.f)
	{
		SetActorRotation(RotationToSet);
		return;
	}

	FRotator LerpRotator = FMath::RInterpTo(GetActorRotation(), RotationToSet, FApp::GetDeltaTime(), LerpSpeed);
	SetActorRotation(LerpRotator);

}

void APlayerCharacter::HandleCameraZoom(float InputValue)
{
	HandleCameraComp->HandleCameraZoom(InputValue);

}

void APlayerCharacter::ZoomLerp()
{


}

