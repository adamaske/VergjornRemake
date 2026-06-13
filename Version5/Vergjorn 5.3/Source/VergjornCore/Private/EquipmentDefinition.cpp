#include "EquipmentDefinition.h"

FPrimaryAssetId UEquipmentDefinition::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(TEXT("EquipmentDef"), GetFName());
}
