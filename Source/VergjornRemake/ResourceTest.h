// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ResourceTest.generated.h"

/**
 * 
 */

enum class ResourceType : uint8;
//enum ResourceType;
UCLASS(BlueprintType, DefaultToInstanced)
class VERGJORNREMAKE_API UResourceTest : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Stuff)
		ResourceType Type;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stuff)
	UTexture2D* Image;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stuff)
	float Amount;
};
