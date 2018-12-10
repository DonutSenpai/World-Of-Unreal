#include "HandleCameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerCharacter.h"

UHandleCameraComponent::UHandleCameraComponent()
{
	//Dothing
}

void UHandleCameraComponent::SetOwningCharacter(class APlayerCharacter* CharacterAttachedTo)
{
	OwnChar = CharacterAttachedTo;
}

void UHandleCameraComponent::SetTargetCameraBoom(class USpringArmComponent* Boom)
{
	TargetBoom = Boom;
}

void UHandleCameraComponent::HandleCameraZoom(float Input)
{
	if (Input != 0)
	{
		if (TargetBoom->TargetArmLength == CameraBoomMaxLength)
			return;

		CameraBoomTargetLength = TargetBoom->TargetArmLength + (CameraZoomSpeed * -Input);
		CameraBoomTargetLength = FMath::Clamp(CameraBoomTargetLength, 0.f, CameraBoomMaxLength);

		GetWorld()->GetTimerManager().SetTimer(CameraZoomLerpHandle, this, &UHandleCameraComponent::CameraZoomLerp, FApp::GetDeltaTime(), true, 0.f);

	}
}

void UHandleCameraComponent::CameraZoomLerp()
{

	NewCameraBoomLength = FMath::FInterpTo(TargetBoom->TargetArmLength, CameraBoomTargetLength, FApp::GetDeltaTime(), 5.f);

	TargetBoom->TargetArmLength = NewCameraBoomLength;

	if (FMath::IsNearlyEqual(NewCameraBoomLength, CameraBoomTargetLength, 0.1f))
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Purple, FString::Printf(TEXT("Fuck shit up: %f"), TargetBoom->TargetArmLength));
		GetWorld()->GetTimerManager().ClearTimer(CameraZoomLerpHandle);
	}

}

void UHandleCameraComponent::BeginPlay()
{
	Super::BeginPlay();
	/*ensureMsgf(TargetBoom != nullptr, TEXT("You need to make sure the TargetBoom is set using the 'SetTargetCameraBoom' function"));
	ensureMsgf(OwnChar != nullptr, TEXT("You need to make sure the owning character reference is set using the 'SetOwningCharacter' function"));
*/

}

