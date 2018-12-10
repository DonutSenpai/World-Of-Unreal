#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ClickableActorInterface.generated.h"

USTRUCT(BlueprintType)
struct FClickableActorInfo
{
	GENERATED_BODY()
	
	FClickableActorInfo() {};

	FClickableActorInfo(class AActor* ClickedActor, FString Name, class UPaperSprite* Portrait, int Level, class UHealthResourceComponent* HealthResource) 
		: ClickedActor(ClickedActor), Name(Name), Portrait(Portrait), Level(Level), HealthResource(HealthResource) {} ;

	UPROPERTY(BlueprintReadOnly)
	class AActor* ClickedActor = nullptr;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FString Name;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UPaperSprite* Portrait = nullptr;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int Level = 0;

	UPROPERTY(BlueprintReadOnly)
	class UHealthResourceComponent* HealthResource = nullptr;

};


UINTERFACE(BlueprintType)
class PROGRAMMING_API UClickableActorInterface : public UInterface
{
	GENERATED_BODY()
};

class PROGRAMMING_API IClickableActorInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Click")
	FClickableActorInfo EventOnClicked();

};