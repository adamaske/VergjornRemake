// Fill out your copyright notice in the Description page of Project Settings.


#include "LoggingHut.h"

#include "GatherableHandler.h"
#include "GatherableUnit.h"
#include "WorkerUnit.h"
#include "PlayerUnit.h"

void ALoggingHut::BeginPlay()
{
	Super::BeginPlay();
}
void ALoggingHut::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IAmWorking())
	{
		Chop(DeltaTime);
	}
};

void ALoggingHut::Chop(float DeltaTime)
{
	for (int i = 0; i < WorkerUnits.Num(); i++)
	{
		switch(WorkerUnits[i]->WorkingState)
		{
			case EWorkState::IDLE:
				//Get them a gatherable unit
				for (int j = 0; j < GatherableUnits.Num(); j++)
				{
					if(GatherableUnits[j]->GetWorker(WorkerUnits[i]))
					{
						WorkerUnits[i]->WorkingState = EWorkState::GATHERING;
						WorkerUnits[i]->GetDestination(GatherableUnits[j]->GetActorLocation());
						break;
					}
				}
				break;
			case EWorkState::GATHERING:
				//Check distance
				if(WorkerUnits[i]->InRange(500.f))
				{
					//Check or add time
					if(WorkerUnits[i]->CurrentTime > ChopTime)
					{
						WorkerUnits[i]->WorkingState = EWorkState::DELIVERING;
						WorkerUnits[i]->GetDestination(GetActorLocation());
						WorkerUnits[i]->CurrentTime = 0;
					}else
					{
						WorkerUnits[i]->CurrentTime += DeltaTime;
					}
				}
				
				break;
			case EWorkState::DELIVERING:
				//Check distance
				if (WorkerUnits[i]->InRange(500.f))
				{
					WorkerUnits[i]->StopMoving();
					//Check or add time
					if (WorkerUnits[i]->CurrentTime >= DropoffTime)
					{
						WorkerUnits[i]->WorkingState = EWorkState::IDLE;
						WorkerUnits[i]->MyGatherableUnit->RemoveWorker(WorkerUnits[i]);
						WorkerUnits[i]->CurrentTime = 0;
						MakeResource();
					}
					else
					{
						WorkerUnits[i]->CurrentTime += DeltaTime;
					}
				}
				break;
			case EWorkState::BUILDING:

				break;
		}
	}
};