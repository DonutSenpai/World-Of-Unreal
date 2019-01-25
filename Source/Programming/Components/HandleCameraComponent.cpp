#include "HandleCameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PlayerCharacter.h"
#include "Debug.h"
#include "MainPlayerController.h"

#define PauseTimerHandle(x) GetWorld()->GetTimerManager().PauseTimer(x)
#define UnpauseTimerHandle(x) GetWorld()->GetTimerManager().UnPauseTimer(x)
#define IsTimerHandleActive(x) GetWorld()->GetTimerManager().IsTimerActive(x)

UHandleCameraComponent::UHandleCameraComponent()
{
	//Dothing
}

void UHandleCameraComponent::BeginPlay()
{
	Super::BeginPlay();
	/*ensureMsgf(TargetBoom != nullptr, TEXT("You need to make sure the TargetBoom is set using the 'SetTargetCameraBoom' function"));
	ensureMsgf(OwnChar != nullptr, TEXT("You need to make sure the owning character reference is set using the 'SetOwningCharacter' function"));
*/

	GetWorld()->GetTimerManager().SetTimer( RotateCameraFromInputHandle, this,
		&UHandleCameraComponent::RotateCameraFromInput, FApp::GetDeltaTime(), true );

	PauseTimerHandle(RotateCameraFromInputHandle);
}

void UHandleCameraComponent::NewMouseState( EMouseState NewState )
{
	CurrentMS = NewState;

	switch (NewState)
	{
	case EMouseState::None:
	{
		if (IsTimerHandleActive(RotateCameraFromInputHandle))
		{
			PauseTimerHandle(RotateCameraFromInputHandle);		
		}

		break;
	}

	case EMouseState::LeftHeld:
	{
		if (!IsTimerHandleActive(RotateCameraFromInputHandle))
		{
			UnpauseTimerHandle(RotateCameraFromInputHandle);
		}

		break;
	}

	case EMouseState::RightHeld:
	{
		if (!IsTimerHandleActive( RotateCameraFromInputHandle ))
		{
			UnpauseTimerHandle( RotateCameraFromInputHandle );
		}

		break;
	}

	case EMouseState::BothHeld:
	{

		if (!IsTimerHandleActive( RotateCameraFromInputHandle ))
		{
			UnpauseTimerHandle( RotateCameraFromInputHandle );
		}

		break;
	}

	}
}

void UHandleCameraComponent::MovementInputRightAxis( float AxisValue )
{
	bInputOnRightAxis = AxisValue != 0;

	if (!bInputOnRightAxis) return;

	if (CurrentMS == EMouseState::None && bInputOnRightAxis && !bInputOnForwardAxis)
	{
		OwnChar->PlayerController->AddYawInput( AxisValue/2.f );
	}


/*
	if (GEngine)
	{

		GEngine->AddOnScreenDebugMessage( -1, 0.f, FColor::Emerald, FString::Printf( TEXT( "Input on RightAxis: %d" ),
			bInputOnRightAxis ) );
	}*/
}

void UHandleCameraComponent::MovementInputForwardAxis( float AxisValue )
{
	bInputOnForwardAxis = AxisValue != 0;

	if (!bInputOnForwardAxis) return;


/*
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage( -1, 0.f, FColor::Green, FString::Printf( TEXT( "Input on ForwardAxis: %d" ),
			bInputOnForwardAxis ) );
	}
*/

}

void UHandleCameraComponent::GetControllerAngleDifference( bool AngleRightOrUp /*= true*/ )
{

}

void UHandleCameraComponent::RotateCameraFromInput()
{
	OwnChar->PlayerController->AddPitchInput(-OwnChar->InputComponent->GetAxisValue( "LookVertical" ));

	OwnChar->PlayerController->AddYawInput(OwnChar->InputComponent->GetAxisValue( "LookHorizontal" ));
}




void UHandleCameraComponent::LerpCameraRotation( FRotator TargetRotation )
{

}

void UHandleCameraComponent::SetOwningCharacter( class APlayerCharacter* CharacterAttachedTo )
{
	OwnChar = CharacterAttachedTo;
}

void UHandleCameraComponent::HandleCameraZoom( float Input )
{
	if (Input != 0)
	{
		if (OwnChar->CameraBoom->TargetArmLength == CameraBoomMaxLength)
			return;

		CameraBoomTargetLength = OwnChar->CameraBoom->TargetArmLength + (CameraZoomSpeed * -Input);
		CameraBoomTargetLength = FMath::Clamp( CameraBoomTargetLength, 0.f, CameraBoomMaxLength );

		GetWorld()->GetTimerManager().SetTimer( CameraZoomLerpHandle, this, &UHandleCameraComponent::CameraZoomLerp, FApp::GetDeltaTime(), true, 0.f );

	}
}

void UHandleCameraComponent::CameraZoomLerp()
{

	NewCameraBoomLength = FMath::FInterpTo( OwnChar->CameraBoom->TargetArmLength, CameraBoomTargetLength, FApp::GetDeltaTime(), 5.f );

	OwnChar->CameraBoom->TargetArmLength = NewCameraBoomLength;

	if (FMath::IsNearlyEqual( NewCameraBoomLength, CameraBoomTargetLength, 0.1f ))
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage( -1, 15.0f, FColor::Purple, FString::Printf( TEXT( "Fuck shit up: %f" ), OwnChar->CameraBoom->TargetArmLength ) );
		GetWorld()->GetTimerManager().ClearTimer( CameraZoomLerpHandle );
	}

}

