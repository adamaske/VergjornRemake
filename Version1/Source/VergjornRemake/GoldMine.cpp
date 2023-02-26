// Fill out your copyright notice in the Description page of Project Settings.


#include "GoldMine.h"
void AGoldMine::BeginPlay()
{
	Super::BeginPlay();
}
void AGoldMine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IAmWorking())
	{
		Mine(DeltaTime);
	}
};

void AGoldMine::Mine(float DeltaTime)
{
	CurrentTime += DeltaTime;

	if (CurrentTime >= MineTime)
	{
		CurrentTime = 0;
		MakeResource();
	}
};