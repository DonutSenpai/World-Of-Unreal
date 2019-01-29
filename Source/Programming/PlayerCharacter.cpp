#include "PlayerCharacter.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "MainPlayerController.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/HandleCameraComponent.h"
#include "Components/HandleCharacterRotationComponent.h"


APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	HandleCameraComp = CreateDefaultSubobject<UHandleCameraComponent>( "HandleCameraComponent" );
	HandleCameraComp->SetupAttachment( RootComponent );
	HandleCameraComp->SetOwningCharacter( this );

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>( TEXT( "CameraBoom" ) );
	CameraBoom->SetupAttachment( RootComponent);

	CameraBoom->TargetArmLength = 650.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>( TEXT( "FollowCamera" ) );
	FollowCamera->SetupAttachment( CameraBoom, USpringArmComponent::SocketName ); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = true; // Camera does not rotate relative to arm


	CharacterRotationComp = CreateDefaultSubobject<UHandleCharacterRotationComponent>( "CharacterRotationComp" );

	MyArrow = CreateDefaultSubobject<UArrowComponent>( "MyArrow" );
	MyArrow->SetupAttachment( RootComponent );
	MyArrow->bHiddenInGame = false;
	MyArrow->ArrowSize = 2.f;

	MyOtherArrow = CreateDefaultSubobject<UArrowComponent>( "MyOtherArrow" );
	MyOtherArrow->SetupAttachment( RootComponent );
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

	PlayerController = Cast<AMainPlayerController>( GetWorld()->GetFirstPlayerController() );
	PlayerController->MouseStateChanged.AddDynamic( this, &APlayerCharacter::NewMouseState );

}

void APlayerCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	//HandleCameraRotation();
	//HandleCharacterRotation();


}

void APlayerCharacter::SetupPlayerInputComponent( class UInputComponent* PlayerInputComponent )
{
	Super::SetupPlayerInputComponent( PlayerInputComponent );
	PlayerInputComponent->BindAxis( "MoveForward", this, &APlayerCharacter::HandleMoveForward );
	PlayerInputComponent->BindAxis( "MoveRight", this, &APlayerCharacter::HandleMoveRight );
	PlayerInputComponent->BindAxis( "LookVertical", this, &APlayerCharacter::HandleLookVertical );
	PlayerInputComponent->BindAxis( "LookHorizontal", this, &APlayerCharacter::HandleLookHorizontal );

	PlayerInputComponent->BindAxis( "CameraZoom", this, &APlayerCharacter::HandleCameraZoom );
	PlayerInputComponent->BindAction( "Jump", IE_Pressed, this, &APlayerCharacter::HandleJump );

}


void APlayerCharacter::Thing( bool yes )
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage( -1, 15.0f, FColor::Purple, FString::Printf( TEXT( "%d THING HAPPNEEEEED! YAAAY!" ), yes ) );
	Jump();
}

void APlayerCharacter::HandleMoveForward( float Value )
{
	HandleCameraComp->MovementInputForwardAxis( Value );
	CharacterRotationComp->MovementInputForwardAxis( Value );
}

void APlayerCharacter::HandleMoveRight( float Value )
{
	HandleCameraComp->MovementInputRightAxis( Value );
	CharacterRotationComp->MovementInputRightAxis( Value );
}

void APlayerCharacter::HandleLookVertical( float Value )
{

}

void APlayerCharacter::HandleLookHorizontal( float Value )
{

}

void APlayerCharacter::HandleJump()
{
	Jump();
}


void APlayerCharacter::NewMouseState( EMouseState NewState )
{
	//HandleCameraComp::Execute_NewMouseState(NewState);

	HandleCameraComp->NewMouseState( NewState );
	CharacterRotationComp->NewMouseState( NewState );

	switch (NewState)
	{
	case EMouseState::None:
	{

		break;
	}

	case EMouseState::LeftHeld:
	{

		break;
	}

	case EMouseState::RightHeld:
	{

		break;
	}

	case EMouseState::BothHeld:
	{
		break;
	}
	}

}

/*
void APlayerCharacter::HandleCameraRotation()
{

	float thing = 6.f;

	if (InputComponent->GetAxisValue( "MoveForward" ) != 0 && PlayerController->CurrentMS == EMouseState::None && !(InputComponent->GetAxisValue( "LookHorizontal" ) != 0))
	{
		float AngleRight = GetControllerAngleDifference( true );
		float AngleUp = FMath::Clamp( GetControllerAngleDifference( false ), 0.f, 20.f );

		FRotator TargetRotation = GetActorForwardVector().RotateAngleAxis( AngleUp, GetActorRightVector() ).ToOrientationRotator();

		FRotator NewControlRotation = FMath::RInterpConstantTo( GetControlRotation(), TargetRotation, FApp::GetDeltaTime(), 250.f );

		PlayerController->SetControlRotation( NewControlRotation );
	}
}*/


void APlayerCharacter::HandleCharacterRotation()
{
	//Calculate things for holding down right
	if (PlayerController->CurrentMS == EMouseState::RightHeld || PlayerController->CurrentMS == EMouseState::BothHeld)
	{
		//resetting variables
		RotationLerpSpeed = 0.f;
		VectorToRotateTo = FVector( 0.f, 0.f, 0.f );

		//Zero out not needed axes and calculate the projections
		ControlYaw = FRotator( 0.f, GetControlRotation().Yaw, 0.f ).Vector();
		float DotProductRight = FVector::DotProduct( ControlYaw, GetActorRightVector() );

		//The calculated angle between this actor and the controller
		float DotProductForward = FVector::DotProduct( ControlYaw, GetActorForwardVector() );
		float Angle = FMath::RadiansToDegrees( FMath::Acos( DotProductForward ) );

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
			VectorToRotateTo = GetActorForwardVector().RotateAngleAxis( DegreesToRotate, FVector( 0.f, 0.f, 1.f ) );
			RotationLerpSpeed = 0.f;
		}

		if (Angle >= MaxControlRotationDifference && !(GetCharacterMovement()->Velocity.Size()))
		{
			if (InputComponent->GetAxisValue( "LookHorizontal" ) != 0)
			{
				DegreesToRotate = (Angle - MaxControlRotationDifference) * CharacterRotationDirection;
				VectorToRotateTo = GetActorForwardVector().RotateAngleAxis( DegreesToRotate, FVector( 0.f, 0.f, 1.f ) );
				RotationLerpSpeed = 45.f;
			}

		}

		if (VectorToRotateTo.Size() > 0)
		{
			SetCharacterRotationInternal( VectorToRotateTo.Rotation(), RotationLerpSpeed );
		}

	}

}

void APlayerCharacter::SetCharacterRotationInternal( FRotator RotationToSet, float LerpSpeed )
{
	if (LerpSpeed <= 0.f)
	{
		SetActorRotation( RotationToSet );
		return;
	}

	FRotator LerpRotator = FMath::RInterpTo( GetActorRotation(), RotationToSet, FApp::GetDeltaTime(), LerpSpeed );
	SetActorRotation( LerpRotator );

}

void APlayerCharacter::HandleCameraZoom( float InputValue )
{
	HandleCameraComp->HandleCameraZoom( InputValue );

}

void APlayerCharacter::ZoomLerp()
{


}

