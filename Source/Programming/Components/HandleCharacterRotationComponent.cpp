#include "HandleCharacterRotationComponent.h"
#include "PlayerCharacter.h"

#define PauseTimerHandle(x) GetWorld()->GetTimerManager().PauseTimer(x)
#define UnpauseTimerHandle(x) GetWorld()->GetTimerManager().UnPauseTimer(x)
#define IsTimerHandleActive(x) GetWorld()->GetTimerManager().IsTimerActive(x)

void UHandleCharacterRotationComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnChar = Cast<APlayerCharacter>( GetOwner() );
	OwnChar;


	GetWorld()->GetTimerManager().SetTimer( RotationHandle, this,
		&UHandleCharacterRotationComponent::SetCharacterRotation, FApp::GetDeltaTime(), true );

	PauseTimerHandle( RotationHandle );

}
void UHandleCharacterRotationComponent::NewMouseState( EMouseState NewState )
{
	EMouseState PreviousState = CurrentMS;
	CurrentMS = NewState;

	switch (NewState)
	{
	case EMouseState::None:
	{
		if (PreviousState == EMouseState::RightHeld)
		{
			GetWorld()->GetTimerManager().SetTimer( RotationHandle, this,
				&UHandleCharacterRotationComponent::LerpToCameraRotation, FApp::GetDeltaTime(), true );
		}

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

void UHandleCharacterRotationComponent::MovementInputRightAxis( float AxisValue )
{

}

void UHandleCharacterRotationComponent::MovementInputForwardAxis( float AxisValue )
{

}


float UHandleCharacterRotationComponent::GetControllerRightAngleDifference()
{
	FVector ControlYaw = FRotator( 0.f, OwnChar->GetControlRotation().Yaw, 0.f ).Vector();
	float DotProductYaw = FVector::DotProduct( ControlYaw, OwnChar->GetActorForwardVector() );
	float DotProductYawRight = FVector::DotProduct( ControlYaw, OwnChar->GetActorRightVector() );
	float AngleRight = FMath::RadiansToDegrees( FMath::Acos( DotProductYaw ) ) * (DotProductYawRight < 0 ? -1 : 1);

	return AngleRight;

	return 0;
}

float UHandleCharacterRotationComponent::GetControllerUpAngleDifference()
{

	FVector ControlPitch = FRotator( OwnChar->GetControlRotation().Pitch, 0.f, 0.f ).Vector();
	float DotProductPitchUp = FVector::DotProduct( ControlPitch, OwnChar->GetActorUpVector() );
	float AngleUp = FMath::RadiansToDegrees( FMath::Acos( DotProductPitchUp ) ) - 90.f;

	return AngleUp;

	return 0;
}

void UHandleCharacterRotationComponent::SetCharacterRotation()
{

}

void UHandleCharacterRotationComponent::LerpCharacterRotation( FRotator TargetRotation )
{

}

void UHandleCharacterRotationComponent::LerpToCameraRotation()
{
	//dot controller forward with character actor forward 


	FVector TargetVec = OwnChar->GetActorForwardVector().RotateAngleAxis( GetControllerRightAngleDifference(), FVector( 0.f, 0.f, 1.f ) );
	FRotator NewRot = FMath::RInterpTo( OwnChar->GetActorRotation(), TargetVec.Rotation(), FApp::GetDeltaTime(), 2.f );


	OwnChar->SetActorRotation( NewRot );
}

