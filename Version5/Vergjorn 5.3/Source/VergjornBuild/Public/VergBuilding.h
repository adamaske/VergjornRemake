#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "VergBuilding.generated.h"

class UBuildingDefinition;
class UStaticMeshComponent;

UENUM(BlueprintType)
enum class EBuildingState : uint8
{
	Ghost,     // Preview during placement
	Planned,   // Placed, awaiting materials
	Hauling,   // Receiving construction materials
	Building,  // Under active construction
	Complete,  // Fully built and operational
};

UCLASS(Abstract, BlueprintType, Blueprintable)
class VERGJORNBUILD_API AVergBuilding : public AActor
{
	GENERATED_BODY()
public:
	AVergBuilding();
	virtual void BeginPlay() override;

	// Called by ConstructionSubsystem on phase transitions
	virtual void OnStateChanged(EBuildingState NewState);

	void SetDefinition(UBuildingDefinition* Def);
	void SetBuildingId(int32 Id) { BuildingId = Id; }

	UFUNCTION(BlueprintCallable) EBuildingState    GetBuildingState() const { return BuildingState; }
	UFUNCTION(BlueprintCallable) UBuildingDefinition* GetDefinition() const { return Definition; }
	UFUNCTION(BlueprintCallable) int32             GetBuildingId()   const { return BuildingId; }

	UPROPERTY(BlueprintReadOnly) FIntPoint GridOrigin;
	UPROPERTY(BlueprintReadOnly) float ConstructionProgress = 0.f;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> MeshComponent;

protected:
	UPROPERTY(BlueprintReadOnly) EBuildingState       BuildingState = EBuildingState::Ghost;
	UPROPERTY(BlueprintReadOnly) TObjectPtr<UBuildingDefinition> Definition;
	UPROPERTY()                  int32                BuildingId    = INDEX_NONE;
};
