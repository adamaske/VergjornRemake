#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "VergSaveable.generated.h"

UINTERFACE(MinimalAPI, BlueprintType)
class UVergSaveable : public UInterface
{
	GENERATED_BODY()
};

class VERGJORNCORE_API IVergSaveable
{
	GENERATED_BODY()
public:
	virtual void SaveState(FArchive& Ar) = 0;
	virtual void LoadState(FArchive& Ar) = 0;
};
