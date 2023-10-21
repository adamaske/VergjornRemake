// Fill out your copyright notice in the Description page of Project Settings.


#include "V_HUD.h"
AV_HUD::AV_HUD()
{
}


void AV_HUD::BeginPlay()
{
	Super::BeginPlay();
}

void AV_HUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
}

void AV_HUD::DrawHUD()
{
	Super::DrawHUD();

	SelectActorsInRectangle();
}


#pragma region Select Actors In Rectangle
void AV_HUD::SelectActorsInRectangle() {
	if (!bSelectActors) {
		return;
	}
	TArray<AActor*> actors;

	GetActorsInSelectionRectangle(mStartAnchor, mEndAnchor, actors, bIncludeNonCollidingComponents, bActorMustBeFullyEnclosed);
	mOnFoundActorsInSelectionRectangle.Broadcast(actors);//Tell everything listening that actors have been selected
	mOnFoundActorsInSelectionRectangle.Clear();//Remove listeners
	
	bSelectActors = false;//Dont select next frame
}

void AV_HUD::StopActorSelection() {
	mOnFoundActorsInSelectionRectangle.Clear();//Remove listeners

	bSelectActors = false;//Dont select next frame
}

//void AV_HUD::MarqueeStart() {
//
//}
//void AV_HUD::MarqueeHeld() {
//
//}
//void AV_HUD::MarqueeEnded() {
//
//}