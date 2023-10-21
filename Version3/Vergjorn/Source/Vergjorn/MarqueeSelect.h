// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MarqueeSelect.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMarqueeSelect : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class VERGJORN_API IMarqueeSelect
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void MarqueeStart();
	virtual void MarqueeHeld();
	virtual void MarqueeEnded();

};
