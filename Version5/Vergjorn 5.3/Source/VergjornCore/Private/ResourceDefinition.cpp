#include "ResourceDefinition.h"

FPrimaryAssetId UResourceDefinition::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("ResourceDef", GetFName());
}
