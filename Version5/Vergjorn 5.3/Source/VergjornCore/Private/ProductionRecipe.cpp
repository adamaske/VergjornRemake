#include "ProductionRecipe.h"

FPrimaryAssetId UProductionRecipe::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("RecipeDef", GetFName());
}
