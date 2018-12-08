#include "MainPlayerController.h"
#include "Components/InputComponent.h"

AMainPlayerController::AMainPlayerController()
{
	bShowMouseCursor = true;
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
}


