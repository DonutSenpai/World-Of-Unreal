#pragma once

#include "UserWidget.h"
#include "SelectedActorWidget.generated.h"

UCLASS()
class PROGRAMMING_API USelectedActorWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	class UClickableActorBaseComponent* SelectedActorComp;
};