#include "VergBuilding.h"
#include "Components/StaticMeshComponent.h"
#include "BuildingDefinition.h"

AVergBuilding::AVergBuilding()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(MeshComponent);
}

void AVergBuilding::BeginPlay()
{
	Super::BeginPlay();
}

void AVergBuilding::SetDefinition(UBuildingDefinition* Def)
{
	Definition = Def;
}

void AVergBuilding::OnStateChanged(EBuildingState NewState)
{
	BuildingState = NewState;
}
