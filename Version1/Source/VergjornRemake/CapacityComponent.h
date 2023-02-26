// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerUnit.h"
#include "CapacityComponent.generated.h"


enum class ResourceType : uint8;
USTRUCT(BlueprintType)
struct FCapacityAmount
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stuff)
		ResourceType myType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stuff)
		float Amount;
};
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VERGJORNREMAKE_API UCapacityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCapacityComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FCapacityAmount> MyAmounts;
};
