
#include "Components/ActorComponent.h"
#include "Interfaces/ClickableActorInterface.h"
#include "ClickableActorBaseComponent.generated.h"

UCLASS(BlueprintType, meta = (BlueprintSpawnableComponent))
class UClickableActorBaseComponent : public UActorComponent, public IClickableActorInterface
{
	GENERATED_BODY()

public:
	UClickableActorBaseComponent();

	UFUNCTION()
	virtual void OnClickedInternal(AActor* TouchedActor, FKey ButtonPressed );

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Values)
	FString Name;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Values)
	class UPaperSprite* Portrait = nullptr;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Values)
	FClickableActorInfo ClickActorInfo;

	UFUNCTION()
	virtual FClickableActorInfo EventOnClicked_Implementation() override;

private:
	void BeginPlay() override;





};