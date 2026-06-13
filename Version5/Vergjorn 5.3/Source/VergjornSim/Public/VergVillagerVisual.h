#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MassEntityTypes.h"
#include "VergVillagerVisual.generated.h"

// Lightweight scene actor driven by the Mass simulation.
// At runtime this is a cylinder+sphere placeholder; swap the mesh in a Blueprint child class.
UCLASS(BlueprintType, Blueprintable)
class VERGJORNSIM_API AVergVillagerVisual : public AActor
{
	GENERATED_BODY()
public:
	AVergVillagerVisual();

	// Root collision capsule (no physics — position is owned by Mass)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Villager|Visual")
	TObjectPtr<class UCapsuleComponent> Capsule;

	// Body — cylinder basic shape, replaceable in Blueprint
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Villager|Visual")
	TObjectPtr<class UStaticMeshComponent> BodyMesh;

	// Head — sphere basic shape, replaceable in Blueprint
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Villager|Visual")
	TObjectPtr<class UStaticMeshComponent> HeadMesh;

	// The Mass entity that drives this actor's transform
	FMassEntityHandle EntityHandle;
};
