#pragma once

#include "Debug.h"
#include "Components/ActorComponent.h"
#include "HandleClickComponent.generated.h"

UCLASS(BlueprintType)
class PROGRAMMING_API UHandleClickComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY()
	class AMainPlayerController* MyPC = nullptr;
	
	UFUNCTION()
	void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void Click(bool Pressed);

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class TSubclassOf<class USelectedActorWidget> SelectedActorWidgetClass;
private:
	class USelectedActorWidget* SelectedActorDisplay;


	void EventOnCompClicked(class UClickableActorBaseComponent* ClickedActorComp);
	class UClickableActorBaseComponent* MouseTrace();

	FTimerHandle TimeSincePressHandle;
	void CountTimeSincePress();
	float Time = 0.f;
};
