// Fill out your copyright notice in the Description page of Project Settings.


#include "V_Entity.h"

V_Entity::V_Entity()
{
	mGUID = FGuid::NewGuid();
}

V_Entity::~V_Entity()
{
}

FGuid V_Entity::GetGUID()
{
	return mGUID;
}

void V_Entity::SetNewGUID(FGuid guid)
{
	mGUID = guid;
}
