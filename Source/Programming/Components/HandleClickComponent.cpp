#include "HandleClickComponent.h"
#include "MainPlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UserWidget.h"
#include "WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

void UHandleClickComponent::BeginPlay()
{
	MyPC->LeftActionButton.AddDynamic(this, &UHandleClickComponent::Click);
	Super::BeginPlay();
	AddOnScreenDebugMsg("Begin Play in handle click comp is run");

}

void UHandleClickComponent::Click(bool Pressed)
{
/*
	AddOnScreenDebugMsg("Clicked");
	if (Pressed)
	{
		Time = 0.f;
		GetWorld()->GetTimerManager().SetTimer(TimeSincePressHandle, this, &UHandleClickComponent::CountTimeSincePress, FApp::GetDeltaTime(), true, 0.f);
	}
	else
	{
		GetWorld()->GetTimerManager().PauseTimer(TimeSincePressHandle);

		if (Time != -1.f)
		{
				
			if (MouseTrace() != nullptr)
			{
				
				EventOnCompClicked(nullptr);
			}
		}
	}*/
}



void UHandleClickComponent::EventOnCompClicked(class UClickableActorBaseComponent* ClickedActorComp)
{
	SelectedActorDisplay = UWidgetBlueprintLibrary::Create(GetWorld(), SelectedActorWidgetClass, MyPC);
		//CreateWidget<TSubclassOf<UUserWidget>>(GetWorld(), SelectedActorWidgetClass);
	SelectedActorDisplay->AddToViewport();

}

UClickableActorBaseComponent* UHandleClickComponent::MouseTrace()
{
	float MouseX, MouseY;
	MyPC->GetMousePosition(MouseX, MouseY);
	FVector2D MouseScreenPos = FVector2D(MouseX, MouseY);

	FVector StartLocation, Direction;

	if (!UGameplayStatics::DeprojectScreenToWorld(MyPC, MouseScreenPos, StartLocation, Direction)) return nullptr;

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%f, %f, %f"), StartLocation.X, StartLocation.Y, StartLocation.Z));
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("%f, %f, %f"), Direction.X, Direction.Y, Direction.Z));
	FHitResult Hit;
	TArray<AActor*> IgnoreActors { };

	if (UKismetSystemLibrary::LineTraceSingleByProfile(GetWorld(), StartLocation, StartLocation + Direction * 100000.f,
		UCollisionProfile::BlockAllDynamic_ProfileName, false, IgnoreActors, EDrawDebugTrace::ForDuration, Hit, true))
	{
		/*if (UClickableActorBaseComponent* HitClickableComp = Hit.GetActor()->FindComponentByClass(UClickableActorBaseComponent))
		{
			return HitClickableComp;
		}*/

	}
	else
	{
		return nullptr;
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
