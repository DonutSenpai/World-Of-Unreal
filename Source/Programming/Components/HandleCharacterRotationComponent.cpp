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
			//GetWorld()->GetTimerManager().SetTimer( RotationHandle, this,
				//&UHandleCharacterRotationComponent::LerpToCameraRotation, FApp::GetDeltaTime(), true );
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
	if (CurrentMS == EMouseState::None)
	{
		float Angle = GetControllerRightAngleDifference();

		if ( Angle < 0 ? FMath::IsNearlyEqual(Angle, -45, 1) : FMath::IsNearlyEqual(Angle, 45, 1) )
		{
			UE_LOG(LogTemp, Warning, TEXT("%f"),Angle/Angle);
		}
		

		FRotator RotationToSet = OwnChar->GetActorRotation() + FRotator( 0.f, 1.f, 0.f ) * AxisValue;
		//RotationToAdd *= AxisValue;

		OwnChar->SetActorRotation( RotationToSet );
	}
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

//This doesn't work yet, but I'll leave it for now
void UHandleCharacterRotationComponent::LerpToCameraRotation()
{
	return;
	//dot controller forward with character actor forward 
/*
	FVector NormalizedControlLookDir = OwnChar->GetControlRotation().Vector();
	NormalizedControlLookDir.Normalize();

	float Vector = FVector::DotProduct(NormalizedControlLookDir, OwnChar->GetActorForwardVector() );
	float DegreesToRotate = FMath::RadiansToDegrees(FMath::Acos(Vector));
	FVector TargetVec = OwnChar->GetActorForwardVector().RotateAngleAxis( DegreesToRotate, FVector( 0.f, 0.f, 1.f ) );

*/




	float RotationValue = GetControllerRightAngleDifference() * -1 * FApp::GetDeltaTime();
	FQuat RotationToAdd = FQuat( FVector( 0.f, 0.f, 1.f ), RotationValue );

	UE_LOG( LogTemp, Warning, TEXT( "%f %f %f %f" ), RotationToAdd.X, RotationToAdd.Y, RotationToAdd.Z, RotationToAdd.W );


	/*
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage( -1, 0.f, FColor::Green, FString::Printf( TEXT( "Controller Rotation: %f %f %f" ),
				OwnChar->GetControlRotation().Pitch, OwnChar->GetControlRotation().Yaw, OwnChar->GetControlRotation().Roll ) );
		}
	*/
	if (OwnChar->GetControlRotation() != OwnChar->GetControlRotation())
	{
		UE_LOG( LogTemp, Warning, TEXT( "Control Rotation: %d %d %d" ), OwnChar->GetControlRotation().Pitch, OwnChar->GetControlRotation().Yaw, OwnChar->GetControlRotation().Roll );
	}

	if (!FMath::IsNearlyEqual( GetControllerRightAngleDifference(), 0, 1.f ))
	{
		//OwnChar->AddActorWorldRotation(RotationToAdd);

	}

	//OwnChar->SetActorRotation( NewRot );
}

