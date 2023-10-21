// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class VERGJORN_API V_GUID
{
public:
	V_GUID();
	~V_GUID();

	FGuid GetGUID();
	void SetNewGUID(FGuid guid);
private:
	FGuid mGUID;
};
