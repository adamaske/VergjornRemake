#include "BuildingDefinition.h"

FPrimaryAssetId UBuildingDefinition::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("BuildingDef", GetFName());
}
