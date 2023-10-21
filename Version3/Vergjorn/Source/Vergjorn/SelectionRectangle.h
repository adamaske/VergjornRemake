// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectionRectangle.generated.h"

/**
 * 
 */
UCLASS()
class VERGJORN_API USelectionRectangle : public UUserWidget
{
	GENERATED_BODY()
public:
	
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int mTest = 0;
};
