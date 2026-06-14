#include "NarrativeEventDefinition.h"

FPrimaryAssetId UNarrativeEventDefinition::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(TEXT("NarrativeDef"), GetFName());
}
