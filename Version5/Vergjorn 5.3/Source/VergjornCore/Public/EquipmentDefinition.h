#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "EquipmentDefinition.generated.h"

UENUM(BlueprintType)
enum class EEquipmentSlot : uint8
{
	Tool,
	Weapon,
	Armor,
};

// Data asset describing one piece of equipment (tool, weapon, or armour).
// Create instances under Content/Data/Equipment/ — the Asset Manager scans that directory.
UCLASS(BlueprintType)
class VERGJORNCORE_API UEquipmentDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Equipment")
	FText DisplayName;

	// Tag that identifies this item at runtime (e.g. TAG_Equipment_Tool_Axe)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Equipment")
	FGameplayTag EquipmentTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Equipment")
	EEquipmentSlot Slot = EEquipmentSlot::Tool;

	// Occupation this item benefits (empty = any)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Equipment")
	FGameplayTag UsableByOccupation;

	// Occupation that must craft this item (usually Blacksmith)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Equipment")
	FGameplayTag CraftedByOccupation;

	// Resources consumed per item crafted
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Equipment|Crafting")
	TMap<FGameplayTag, int32> CraftingCost;

	// Work units a crafter must spend to produce one item
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Equipment|Crafting",
		meta=(ClampMin="1"))
	float CraftWorkUnits = 50.f;

	// Multiplier applied to a villager's work rate when this item is equipped.
	// 1.0 = no bonus. Typical tools: 1.5. Weapons add combat value, not work speed.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Equipment|Stats",
		meta=(ClampMin="0.1", ClampMax="5.0"))
	float WorkEfficiencyMultiplier = 1.5f;

	// Mesh attached to the villager visual when equipped (soft ref — not always loaded)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Equipment|Visual")
	TSoftObjectPtr<UStaticMesh> EquippedMesh;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
