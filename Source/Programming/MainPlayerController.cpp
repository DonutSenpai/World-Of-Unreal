#include "MainPlayerController.h"
#include "Components/InputComponent.h"
#include "Components/HandleClickComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/ClickableActorBaseComponent.h"

AMainPlayerController::AMainPlayerController()
{
	bShowMouseCursor = true;
	HandleClick = CreateDefaultSubobject<UHandleClickComponent>("HandleClick");
	HandleClick->MyPC = this;

}


void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("RightActionButton", IE_Pressed, this, &AMainPlayerController::RightActionButtonPressedInternal);
	InputComponent->BindAction("RightActionButton", IE_Released, this, &AMainPlayerController::RightActionButtonReleasedInternal);

	InputComponent->BindAction("LeftActionButton", IE_Pressed, this, &AMainPlayerController::LeftActionButtonPressedInternal);
	InputComponent->BindAction("LeftActionButton", IE_Released, this, &AMainPlayerController::LeftActionButtonReleasedInternal);
}

void AMainPlayerController::SetShowMouse(bool Show)
{
	if (Show && CurrentMS == EMouseState::None)
	{
		SetMouseLocation(MouseX, MouseY);
		bShowMouseCursor = true;
	}
	else if (!Show)
	{
		if (CurrentMS == EMouseState::BothHeld)
			return;

		GetMousePosition(MouseX, MouseY);
		bShowMouseCursor = false;
	}
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetInputMode(FInputModeGameOnly());
}


void AMainPlayerController::RightActionButtonPressedInternal()
{
	
	bHoldsRight = true;
	if (CurrentMS == EMouseState::LeftHeld)
		CurrentMS = EMouseState::BothHeld;

	else if (CurrentMS == EMouseState::None)
		CurrentMS = EMouseState::RightHeld;

	SetShowMouse(false);
	RightActionButton.Broadcast(true);
}

void AMainPlayerController::RightActionButtonReleasedInternal()
{
	bHoldsRight = false;
	if (CurrentMS == EMouseState::BothHeld)
		CurrentMS = EMouseState::LeftHeld;
	
	else if (CurrentMS == EMouseState::RightHeld)
		CurrentMS = EMouseState::None;


	SetShowMouse(true);
	RightActionButton.Broadcast(false);
}

void AMainPlayerController::LeftActionButtonPressedInternal()
{
	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Handle Left Action Button In Player Controll cpp file is fired"));
	*/
	bHoldsLeft = true;
	if (CurrentMS == EMouseState::RightHeld)
		CurrentMS = EMouseState::BothHeld;

	else if (CurrentMS == EMouseState::None)
		CurrentMS = EMouseState::LeftHeld;

	SetShowMouse(false);
	LeftActionButton.Broadcast(true);
}

void AMainPlayerController::LeftActionButtonReleasedInternal()
{
	bHoldsLeft = false;

	if (CurrentMS == EMouseState::BothHeld)
		CurrentMS = EMouseState::RightHeld;

	else if (CurrentMS == EMouseState::LeftHeld)
		CurrentMS = EMouseState::None;


	SetShowMouse(true);
	LeftActionButton.Broadcast(false);
	//MouseTraceInternal();
}

UClickableActorBaseComponent* AMainPlayerController::MouseTraceInternal()
{
	FVector StartLocation, Direction;

	if (!DeprojectMousePositionToWorld(StartLocation, Direction)) return nullptr;

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%f, %f, %f"), StartLocation.X, StartLocation.Y, StartLocation.Z));
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("%f, %f, %f"), Direction.X, Direction.Y, Direction.Z));
	FHitResult Hit;
	TArray<AActor*> IgnoreActors { };

	if (UKismetSystemLibrary::LineTraceSingleByProfile(GetWorld(), StartLocation, StartLocation + Direction * 100000.f,
		UCollisionProfile::BlockAllDynamic_ProfileName, false, IgnoreActors, EDrawDebugTrace::ForDuration, Hit, true))
	{
		if (UClickableActorBaseComponent* HitClickableComp = Hit.GetActor()->FindComponentByClass<UClickableActorBaseComponent>())
		{
			return HitClickableComp;
		}
	
	}
	else
	{
		return nullptr;
	}

	return nullptr;
}

