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

	OwnChar = Cast<APlayerCharacter>(GetOwner());
	OwnChar;

}

void UHandleCharacterRotationComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	HandleRotation();

	UE_LOG(LogTemp, Warning, TEXT("%f"), GetControllerRightAngleDifference());
}

//Needs debugging and testing and fixing, this whole file isnt done yet
void UHandleCharacterRotationComponent::HandleRotation()
{
	switch (RotationAction)
	{

	case ECharacterRotationAction::E_None:
	{
		break;
	}

	case ECharacterRotationAction::E_InheritControlRotation:
	{
		if (!OwnChar->bUseControllerRotationYaw)
		{
			if (!FMath::IsNearlyEqual(GetControllerRightAngleDifference(), 0.f, 0.01f))
			{
				RotateToCamera();
			}
			else
			{
				OwnChar->bUseControllerRotationYaw = true;
			}

		}

		break;
	}

	case ECharacterRotationAction::E_AddRotation:
	{
		AddRotation();
		break;
	}

	case ECharacterRotationAction::E_RotateAtAngle:
	{
		RotateAtAngle(45.f);
		break;
	}

	case ECharacterRotationAction::E_RotateToAngle45:
	{
		RotateToAngle(45.f);
		break;
	}

	case ECharacterRotationAction::E_RotateToAngle90:
	{
		RotateToAngle(90.f);
		break;
	}

	case ECharacterRotationAction::E_RotateToCamera:
	{
		if (FMath::IsNearlyEqual(GetControllerRightAngleDifference(), 0.f, 0.01f))
		{
			RotationAction = ECharacterRotationAction::E_None;
		}

		RotateToCamera();
		break;
	}

	}
}



void UHandleCharacterRotationComponent::RightMovAxisPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("Right Is Pressed"));

	if (CurrentMS == EMouseState::None)
	{
		RotationAction = ECharacterRotationAction::E_RotateAtAngle;
	}

	if (CurrentMS == EMouseState::LeftHeld)
	{
		RotationAction = ECharacterRotationAction::E_AddRotation;
	}

	if (CurrentMS == EMouseState::RightHeld)
	{
		RotationAction = ECharacterRotationAction::E_RotateToAngle90;
	}

	if (CurrentMS == EMouseState::BothHeld || (CurrentMS == EMouseState::RightHeld && bInputOnForwardAxis))
	{
		RotationAction = ECharacterRotationAction::E_RotateToAngle45;
	}

}

void UHandleCharacterRotationComponent::RightMovAxisReleased()
{
	UE_LOG(LogTemp, Warning, TEXT("Right Is Released"));

	if (CurrentMS == EMouseState::None)
	{
		RotationAction = ECharacterRotationAction::E_RotateToCamera;
	}
	else
	{
		//Reset the Rotation action to be what it was before the button was pressed
		NewMouseState(CurrentMS);
	}

}

void UHandleCharacterRotationComponent::ForwardMovAxisPressed()
{
	if (bInputOnRightAxis && CurrentMS == EMouseState::None)
	{
		RotationAction = ECharacterRotationAction::E_AddRotation;
	}

	if (!bInputOnRightAxis && CurrentMS == EMouseState::RightHeld)
	{
		RotationAction = ECharacterRotationAction::E_InheritControlRotation;
	}

	if (bInputOnRightAxis && (CurrentMS == EMouseState::RightHeld || CurrentMS == EMouseState::BothHeld))
	{
		RotationAction = ECharacterRotationAction::E_RotateToAngle45;
	}

}

void UHandleCharacterRotationComponent::RightActionButtonReleased()
{
	if (CurrentMS == EMouseState::None && !bInputOnRightAxis && !bInputOnForwardAxis)
	{
		RotationAction = ECharacterRotationAction::E_RotateToCamera;
	}
}

void UHandleCharacterRotationComponent::NewMouseState(EMouseState NewState)
{
	PreviousMS = CurrentMS;
	CurrentMS = NewState;

	if (PreviousMS == EMouseState::BothHeld)
	{
		OwnChar->bUseControllerRotationYaw = false;
	}

	switch (NewState)
	{
	case EMouseState::None:
	{
		if (!bInputOnForwardAxis && bInputOnRightAxis)
		{
			RotationAction = ECharacterRotationAction::E_RotateAtAngle;
		}

		if (bInputOnRightAxis && bInputOnForwardAxis)
		{
			RotationAction = ECharacterRotationAction::E_AddRotation;
		}

		break;
	}

	case EMouseState::LeftHeld:
	{
		if (bInputOnRightAxis)
		{
			RotationAction = ECharacterRotationAction::E_AddRotation;
		}
		else
		{
			RotationAction = ECharacterRotationAction::E_None;
		}
		

		break;
	}

	case EMouseState::RightHeld:
	{
		if (!bInputOnForwardAxis && !bInputOnRightAxis)
		{
			RotationAction = ECharacterRotationAction::E_RotateAtAngle;
		}
		else if (bInputOnRightAxis)
		{
			RotationAction = bInputOnForwardAxis ? ECharacterRotationAction::E_RotateToAngle45 :
				ECharacterRotationAction::E_RotateToAngle90;
		}

		break;
	}

	case EMouseState::BothHeld:
	{
		if (!bInputOnRightAxis && !bInputOnForwardAxis)
		{
			RotationAction = ECharacterRotationAction::E_InheritControlRotation;
		}
		else if (bInputOnRightAxis)
		{
			RotationAction = ECharacterRotationAction::E_RotateToAngle45;
		}

		break;
	}

	}
}

void UHandleCharacterRotationComponent::MovementInputRightAxis(float AxisValue)
{
	bInputOnRightAxis = AxisValue != 0.f;
}

void UHandleCharacterRotationComponent::MovementInputForwardAxis(float AxisValue)
{
	bInputOnForwardAxis = AxisValue != 0;
}


//Need to test this
void UHandleCharacterRotationComponent::RotateToCamera()
{
	FRotator TargetRotation = OwnChar->GetActorForwardVector().RotateAngleAxis
	(GetControllerRightAngleDifference(), FVector(0.f, 0.f, 1.f)).Rotation();

	LerpSetRotation(TargetRotation);
}

//Need to test this
void UHandleCharacterRotationComponent::RotateToAngle(float Degrees)
{
	float Direction = OwnChar->InputComponent->GetAxisValue("MoveRight");

	float DegreesToRotate = FMath::Abs(GetControllerRightAngleDifference()) - FMath::Abs(Degrees);
	DegreesToRotate = DegreesToRotate * Direction;

	FRotator TargetRotation = OwnChar->GetActorForwardVector().RotateAngleAxis(DegreesToRotate,
		FVector(0.f, 0.f, 1.f)).Rotation();

	LerpSetRotation(TargetRotation);
}

//Need to test this
void UHandleCharacterRotationComponent::RotateAtAngle(float Degrees)
{
	//UE_LOG( LogTemp, Warning, TEXT( "AngDifConstant: %f" ), GetControllerRightAngleDifference() );
	if (FMath::Abs(GetControllerRightAngleDifference()) >= Degrees)
	{
		//UE_LOG( LogTemp, Warning, TEXT( "AngDif: %f" ), GetControllerRightAngleDifference() );

		//Calculate the degrees it needs to rotate to keep a @param Degrees angle from the controller
		float AngDif = GetControllerRightAngleDifference();
		AngDif = FMath::Abs(GetControllerRightAngleDifference());
		AngDif = AngDif - Degrees;
		AngDif = AngDif * (GetControllerRightAngleDifference() / FMath::Abs(GetControllerRightAngleDifference()));

		//Using Vectors is easier than manipulating rotators directly
		FRotator TargetRotation = OwnChar->GetActorForwardVector().RotateAngleAxis(AngDif, FVector(0.f, 0.f, 1.f)).Rotation();

		if (FMath::IsNearlyEqual(AngDif, 0.f, 0.01f)) return;
		
		OwnChar->SetActorRotation(TargetRotation);
		//LerpSetRotation(TargetRotation);

	}
}

//Need to test this
void UHandleCharacterRotationComponent::AddRotation()
{

	float Direction = FApp::GetDeltaTime() * OwnChar->InputComponent->GetAxisValue("MoveRight");

	OwnChar->AddActorWorldRotation(FRotator(0.f, Direction, 0.f));
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

void UHandleCharacterRotationComponent::LerpSetRotation(FRotator TargetRotation)
{
	FRotator NewRotation = FMath::RInterpTo(OwnChar->GetActorRotation(), TargetRotation,
		FApp::GetDeltaTime(), LerpSpeed);

	OwnChar->SetActorRotation(NewRotation);
}

float UHandleCharacterRotationComponent::GetControllerRightAngleDifference()
{
	FVector ControlYaw = FRotator(0.f, OwnChar->GetControlRotation().Yaw, 0.f).Vector();
	float DotProductYaw = FVector::DotProduct(ControlYaw, OwnChar->GetActorForwardVector());
	float DotProductYawRight = FVector::DotProduct(ControlYaw, OwnChar->GetActorRightVector());
	float AngleRight = FMath::RadiansToDegrees(FMath::Acos(DotProductYaw)) * (DotProductYawRight < 0 ? -1 : 1);

	return AngleRight;

	return 0;
}

float UHandleCharacterRotationComponent::GetControllerUpAngleDifference()
{

	FVector ControlPitch = FRotator(OwnChar->GetControlRotation().Pitch, 0.f, 0.f).Vector();
	float DotProductPitchUp = FVector::DotProduct(ControlPitch, OwnChar->GetActorUpVector());
	float AngleUp = FMath::RadiansToDegrees(FMath::Acos(DotProductPitchUp)) - 90.f;

	return AngleUp;

	return 0;
}
