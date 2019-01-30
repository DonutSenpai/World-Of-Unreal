#include "HandleCharacterRotationComponent.h"
#include "PlayerCharacter.h"
#include "TimerManager.h"

#define PauseTimerHandle(x) GetWorld()->GetTimerManager().PauseTimer(x)
#define UnpauseTimerHandle(x) GetWorld()->GetTimerManager().UnPauseTimer(x)
#define IsTimerHandleActive(x) GetWorld()->GetTimerManager().IsTimerActive(x)

UHandleCharacterRotationComponent::UHandleCharacterRotationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

}

void UHandleCharacterRotationComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnChar = Cast<APlayerCharacter>( GetOwner() );
	OwnChar;



	GetWorld()->GetTimerManager().SetTimer( RotationHandle, this,
		&UHandleCharacterRotationComponent::SetCharacterRotation, FApp::GetDeltaTime(), true );

	PauseTimerHandle( RotationHandle );


}

void UHandleCharacterRotationComponent::TickComponent( float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	HandleRotation();


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

void UHandleCharacterRotationComponent::NewMouseState( EMouseState NewState )
{
	PreviousMS = CurrentMS;
	CurrentMS = NewState;

	if (NewState != EMouseState::None)
	{
		bRotateToCamera = false;
	}


	switch (NewState)
	{
	case EMouseState::None:
	{
		if (PreviousMS == EMouseState::RightHeld)
		{
			bRotateToCamera = true;
			/*GetWorld()->GetTimerManager().SetTimer( RotationHandle, this,
				&UHandleCharacterRotationComponent::LerpToCameraRotation, FApp::GetDeltaTime(), true );*/


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
	bInputOnRightAxis = AxisValue != 0;
}

void UHandleCharacterRotationComponent::MovementInputForwardAxis( float AxisValue )
{
	bInputOnRightAxis = AxisValue != 0;
}



void UHandleCharacterRotationComponent::SetCharacterRotation()
{

}

void UHandleCharacterRotationComponent::RotateDegrees( float Degrees )
{
	FRotator TargetRotation = OwnChar->GetActorForwardVector().RotateAngleAxis( Degrees,
		FVector( 0.f, 0.f, 1.f ) ).Rotation();


	LerpSetRotation( TargetRotation );
}


void UHandleCharacterRotationComponent::RotateKeepDegrees( float Degrees )
{
	UE_LOG( LogTemp, Warning, TEXT( "AngDifConstant: %f" ), GetControllerRightAngleDifference() );
	if (FMath::Abs( GetControllerRightAngleDifference() ) >= Degrees)
	{
		UE_LOG( LogTemp, Warning, TEXT( "AngDif: %f" ), GetControllerRightAngleDifference() );

		//Calculate the degrees it needs to rotate to keep a @param Degrees angle from the controller
		float AngDif = GetControllerRightAngleDifference();
		AngDif = FMath::Abs( GetControllerRightAngleDifference() );
		AngDif = AngDif - Degrees;
		AngDif = AngDif * (GetControllerRightAngleDifference() / FMath::Abs( GetControllerRightAngleDifference() ));

		//Using Vectors is easier than manipulating rotators directly
		FRotator TargetRotation = OwnChar->GetActorForwardVector().RotateAngleAxis( AngDif, FVector( 0.f, 0.f, 1.f ) ).Rotation();

		LerpSetRotation( TargetRotation );

	}
}

//This doesn't work yet, but I'll leave it for now
void UHandleCharacterRotationComponent::HandleRotation()
{
	if (CurrentMS == EMouseState::None)
	{
		
	}


	if (CurrentMS == EMouseState::RightHeld || bInputOnRightAxis)
	{
		RotateKeepDegrees( 45.f );
	}


	if (CurrentMS == EMouseState::None && (!bInputOnRightAxis && !bInputOnForwardAxis) 
		|| PreviousMS == EMouseState::RightHeld)
	{


		RotateKeepDegrees(0.f);
	
	}

	return;

}

/*


		float RotationValue = GetControllerRightAngleDifference() * -1 * FApp::GetDeltaTime();
	FQuat RotationToAdd = FQuat( FVector( 0.f, 0.f, 1.f ), RotationValue );

	UE_LOG( LogTemp, Warning, TEXT( "%f %f %f %f" ), RotationToAdd.X, RotationToAdd.Y, RotationToAdd.Z, RotationToAdd.W );


	/ *
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage( -1, 0.f, FColor::Green, FString::Printf( TEXT( "Controller Rotation: %f %f %f" ),
				OwnChar->GetControlRotation().Pitch, OwnChar->GetControlRotation().Yaw, OwnChar->GetControlRotation().Roll ) );
		}
	* /
	if (OwnChar->GetControlRotation() != OwnChar->GetControlRotation())
	{
		UE_LOG( LogTemp, Warning, TEXT( "Control Rotation: %d %d %d" ), OwnChar->GetControlRotation().Pitch,
			OwnChar->GetControlRotation().Yaw, OwnChar->GetControlRotation().Roll );
	}

	if (!FMath::IsNearlyEqual( GetControllerRightAngleDifference(), 0, 1.f ))
	{
		//OwnChar->AddActorWorldRotation(RotationToAdd);

	}

	LerpSetRotation( RotationToAdd.Rotator() );

	//OwnChar->SetActorRotation( NewRot );
}*/

void UHandleCharacterRotationComponent::LerpSetRotation( FRotator TargetRotation )
{

	LerpAlpha += FApp::GetDeltaTime() / 2;

	if (LerpAlpha >= 1.f)
	{
		LerpAlpha = 0.f;
	}

	FRotator NewRotation = FMath::RInterpTo( OwnChar->GetActorRotation(), TargetRotation, FApp::GetDeltaTime(), LerpSpeed );

	OwnChar->SetActorRotation( NewRotation );


}
