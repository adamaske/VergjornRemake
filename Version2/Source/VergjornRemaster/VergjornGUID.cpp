// Fill out your copyright notice in the Description page of Project Settings.


#include "VergjornGUID.h"

VergjornGUID::VergjornGUID()
{
	mGUID = FGuid::NewGuid();
}

VergjornGUID::~VergjornGUID()
{
}

FGuid VergjornGUID::GetGUID()
{
	return mGUID;
}
