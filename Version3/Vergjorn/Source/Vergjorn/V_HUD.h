// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MarqueeSelect.h"
#include "V_HUD.generated.h"

/**
 * 
 */
//Delegate that broadcast the actors found in selection rectangle
DECLARE_MULTICAST_DELEGATE_OneParam(FOnFoundActorsInSelectionRectangle, TArray<AActor*> /* outActors */);

class IMarqueeSelect;

UCLASS()
class VERGJORN_API AV_HUD : public AHUD, public IMarqueeSelect
{
	GENERATED_BODY()
public:
	AV_HUD();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	// Primary draw call for the HUD.
	virtual void DrawHUD() override;

	//======SELECT ACTORS IN RECTANGLE==========
	void SelectActorsInRectangle();
	//Delegate that broadcasts upon selection rectangle functions
	FOnFoundActorsInSelectionRectangle mOnFoundActorsInSelectionRectangle;
	//Returns the delegate that braodcasts on completion
	//Bind functions to this delegate, the bound fucntion must have TArray<AActor*> as a parameter
	//Beware this delegate will not instantly complete, it will happend next DrawHUD call.
	template<typename T>
	void SelectActorsInRectangle(T* InUserObject, void(T::*InFunc)(TArray<AActor*, FDefaultAllocator>), FVector2D start, FVector2D end, bool IncludeNonCollidingComponents, bool ActorMustBeFullyEnclosed);

	void StopActorSelection();//Incase of canceled selection, clear 
	bool bSelectActors = false;//Select actors next HUD call
	FVector2D mStartAnchor = FVector2D{ 0,0 };//Start 
	FVector2D mEndAnchor = FVector2D{0,0};//End
	bool bIncludeNonCollidingComponents = true;
	bool bActorMustBeFullyEnclosed = true;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void MarqueeStart() override;
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void MarqueeHeld() override;
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void MarqueeEnded() override;
};

template<typename T>
void AV_HUD::SelectActorsInRectangle(T* InUserObject, void(T::* InFunc)(TArray<AActor*, FDefaultAllocator>),
	FVector2D start, FVector2D end, bool IncludeNonCollidingComponents, bool ActorMustBeFullyEnclosed)
{
	bSelectActors = true;//Select actors next tick
	mStartAnchor = start;
	mEndAnchor = end;
	bIncludeNonCollidingComponents = IncludeNonCollidingComponents;
	bActorMustBeFullyEnclosed = ActorMustBeFullyEnclosed;

	mOnFoundActorsInSelectionRectangle.AddUObject(InUserObject, InFunc);
}
