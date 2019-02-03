#pragma once


#include "GameFramework/PlayerController.h"
#include "MouseStateEnum.h"
#include "MovementAxisStateEnum.h"
#include "MainPlayerController.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FActionButtonPressed, bool, Pressed );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnMouseStateChanged, EMouseState, NewState );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnMovAxisStateChanged, EMovementAxisState, NewState );

UCLASS()
class PROGRAMMING_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

	AMainPlayerController();

public:
	void SetupInputComponent() override;

	void BeginPlay() override;

	UFUNCTION( BlueprintCallable )
	void SetShowMouse( bool Show );

	//GetInputMouseDelta - Retrieves how far the mouse moved this frame
	//Maybe use some of this when calculating character-rotation to only rotate when the mouse is at the edge of the screen

	FActionButtonPressed RightActionButton;
	FActionButtonPressed LeftActionButton;
	FOnMouseStateChanged MouseStateChanged;
	FOnMovAxisStateChanged MovAxisStateChanged;

	EMouseState CurrentMS = EMouseState::None;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	EMovementAxisState CurrentMovAxis = EMovementAxisState::None;

	bool bHoldsRight = false;
	bool bHoldsLeft = false;

	class UClickableActorBaseComponent* MouseTraceInternal();

	UPROPERTY( BlueprintReadOnly, VisibleAnywhere )
	class UHandleClickComponent* HandleClick = nullptr;

private:
	float MouseX = 0.f;
	float MouseY = 0.f;

	void RightActionButtonPressedInternal();
	void RightActionButtonReleasedInternal();
	void LeftActionButtonPressedInternal();
	void LeftActionButtonReleasedInternal();

	void RightMovAxisPressedInternal();
	void RightMovAxisReleasedInternal();

	void ForwardMovAxisPressedInternal();
	void ForwardMovAxisReleasedInternal();



};


