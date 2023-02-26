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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
		float MineTime{ 10 };
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
		float CurrentTime{ 0 };

	void Mine(float);
};
