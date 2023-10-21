// Fill out your copyright notice in the Description page of Project Settings.


#include "V_GUID.h"

V_GUID::V_GUID()
{
	mGUID = FGuid::NewGuid();
}

V_GUID::~V_GUID()
{
}

FGuid V_GUID::GetGUID() {
	return mGUID;
}

void V_GUID::SetNewGUID(FGuid guid) {
	mGUID = guid;
}