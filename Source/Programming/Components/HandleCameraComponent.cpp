#include "HandleCameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PlayerCharacter.h"
#include "Debug.h"
#include "MainPlayerController.h"

UHandleCameraComponent::UHandleCameraComponent()
{
	//Dothing
}

void UHandleCameraComponent::NewMouseState( EMouseState NewState )
{
	CurrentMS = NewState;

	switch (NewState)
	{
	case EMouseState::None:
	{
		break;
	}

	case EMouseState::LeftHeld:
	{
		GetWorld()->GetTimerManager().SetTimer(RotateCameraFromInputHandle, this, 
			&UHandleCameraComponent::RotateCameraFromInput, FApp::GetDeltaTime(), true);
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




void UHandleCameraComponent::MovementInputOnAxis( EInputAxis InputOnAxis, float AxisValue )
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

void UHandleCameraComponent::BeginPlay()
{
	Super::BeginPlay();
	/*ensureMsgf(TargetBoom != nullptr, TEXT("You need to make sure the TargetBoom is set using the 'SetTargetCameraBoom' function"));
	ensureMsgf(OwnChar != nullptr, TEXT("You need to make sure the owning character reference is set using the 'SetOwningCharacter' function"));
*/

}

