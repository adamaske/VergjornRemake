// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class VERGJORNREMASTER_API VergjornGUID
{
public:
	VergjornGUID();
	~VergjornGUID();

	FGuid GetGUID();

private:
	FGuid mGUID;
};
