#include "ClickableActorBaseComponent.h"
#include "GameFramework/Actor.h"

UClickableActorBaseComponent::UClickableActorBaseComponent()
{
}

void UClickableActorBaseComponent::OnClickedInternal(AActor* TouchedActor, FKey ButtonPressed)
{
	/*if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, (TouchedActor->GetName()));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString::Printf(TEXT("On Clicked Event Happened on actor:")));
	}*/
}

FClickableActorInfo UClickableActorBaseComponent::EventOnClicked_Implementation()
{
	//Gonna add more functionality soon
	return ClickActorInfo;
}

void UClickableActorBaseComponent::BeginPlay()
{
	Super::BeginPlay();
	GetOwner()->OnClicked.AddUniqueDynamic(this, &UClickableActorBaseComponent::OnClickedInternal);
}
