// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Structure.h"
#include "GoldMine.generated.h"

/**
 * 
 */
UCLASS()
class VERGJORNREMAKE_API AGoldMine : public AStructure
{
	GENERATED_BODY()
	
public:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float MineTime{ 10 };
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float CurrentTime{ 10 };

	void Mine(float);
};
