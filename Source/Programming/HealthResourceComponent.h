#pragma once

#include "Components/ActorComponent.h"
#include "HealthResourceComponent.generated.h"

UENUM(BlueprintType)
enum class EResourceType : uint8
{
	Mana,
	Energy,
	Rage
};

UCLASS(BlueprintType, meta = (BlueprintSpawnableComponent))
class UHealthResourceComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealthResourceComponent(const FObjectInitializer& ObjectInitializer/*, AActor* Owner*/);

	//AActor* Owner = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = Values)
	float Health = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Values)
	float MaxHealth = 100.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Values)
	EResourceType ResourceType = EResourceType::Mana;

	UPROPERTY(BlueprintReadWrite, Category = Values)
	float Resource = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Values)
	float MaxResource = 100.f;

	//virtual void BeginPlay() override;



};