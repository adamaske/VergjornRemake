#include "OccupationDefinition.h"

FPrimaryAssetId UOccupationDefinition::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("OccupationDef", GetFName());
}
