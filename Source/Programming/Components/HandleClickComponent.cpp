#include "HandleClickComponent.h"
#include "MainPlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UserWidget.h"
#include "WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "ClickableActorBaseComponent.h"

void UHandleClickComponent::BeginPlay()
{
	MyPC = Cast<AMainPlayerController>(GetOwner());
	checkf(MyPC != nullptr, TEXT("HandleClickComponent is for use in the MainPlayerController only"));

	MyPC->LeftActionButton.AddDynamic(this, &UHandleClickComponent::Click);
	Super::BeginPlay();
	AddOnScreenDebugMsg("Begin Play in handle click comp is run");

}

void UHandleClickComponent::Click(bool Pressed)
{
	

	if (Pressed)
	{
		AddOnScreenDebugMsg("Clicked");
		Time = 0.f;
		GetWorld()->GetTimerManager().SetTimer(TimeSincePressHandle, this, &UHandleClickComponent::CountTimeSincePress, FApp::GetDeltaTime(), true, 0.f);
	}
	else
	{
		AddOnScreenDebugMsg("Released");
		GetWorld()->GetTimerManager().PauseTimer(TimeSincePressHandle);

		if (Time != -1.f)
		{
				
			if (MouseTrace() != nullptr)
			{
				
				EventOnCompClicked(nullptr);
			}
		}
	}
}



void UHandleClickComponent::EventOnCompClicked(class UClickableActorBaseComponent* ClickedActorComp)
{
	SelectedActorDisplay = UWidgetBlueprintLibrary::Create(GetWorld(), SelectedActorWidgetClass, MyPC);
	SelectedActorDisplay->AddToViewport();

}

UClickableActorBaseComponent* UHandleClickComponent::MouseTrace()
{

	FVector StartLocation, Direction;

	if (!MyPC->DeprojectMousePositionToWorld(StartLocation, Direction)) return nullptr;

	FHitResult Hit;

	TArray<AActor*> IgnoreActors { };
	
	if (UKismetSystemLibrary::LineTraceSingleByProfile(GetWorld(), StartLocation, StartLocation + Direction * 100000.f,
		UCollisionProfile::BlockAllDynamic_ProfileName, false, IgnoreActors, EDrawDebugTrace::ForDuration, Hit, true) )
	{
		UClickableActorBaseComponent* HitClickableComp = Hit.GetActor()->FindComponentByClass<UClickableActorBaseComponent>();

		if (HitClickableComp)
		{
			return HitClickableComp;
		}
		else
		{
			return nullptr;
		}

	}

	return nullptr;

}

void UHandleClickComponent::CountTimeSincePress()
{
	Time += FApp::GetDeltaTime();
	if (Time >= 0.2f)
	{
		Time = -1.f;
		GetWorld()->GetTimerManager().PauseTimer(TimeSincePressHandle);
	}
}
