// Fill out your copyright notice in the Description page of Project Settings.


#include "EntitySelector.h"

#include "Kismet/GameplayStatics.h"
#include "V_HUD.h"

//Test
#include "Engine/StaticMeshActor.h"
// Sets default values
AEntitySelector::AEntitySelector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEntitySelector::BeginPlay()
{
	Super::BeginPlay();


	//TESTING 
	CreateSelectionRequest(FSelectionGroup(FString{ "Testing" }, FString{ "Desc" }, AActor::StaticClass()), this, &AEntitySelector::TestingChangedOrCompleted);


	//CreateSelectionRequest(FSelectionGroup(FString{ "Test 2" }, FString{ "Desc" }, AActor::StaticClass()));
	//bool equal = mSelectionGroups[0].Get() == mSelectionGroups[1].Get();
}
// Called every frame
void AEntitySelector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bSelecting) {
		WhileSelection();
	}
}

void AEntitySelector::SetAddative(float value)
{
	mAddativeAxis = value;
}

void AEntitySelector::StartSelection()
{
	//TODO - change MousePos anchors to projected world location points
	mStartAnchor = GetMousePosition();//Get the current mouse position

	//TODO Linetrace toward cursor to get what is benath it and add to selection

	AddActorToSelection(GetActorUnderCursor());
	bSelecting = true;
}

void AEntitySelector::WhileSelection()
{
	
}

void AEntitySelector::StopSelection()
{
	if (!bSelecting) {
		CancelSelection();
		return;
	}
	auto pc = GetWorld()->GetFirstPlayerController();
	if (!pc) {
		CancelSelection();
		return;
	}
	auto hud = Cast<AV_HUD>(pc->GetHUD());
	if (!hud) {
		CancelSelection();
		return;
	}

	AddActorToSelection(GetActorUnderCursor());

	//Stop drawing rectangle
	mEndAnchor = GetMousePosition();
	
	//Tell hud to select actors and get delegate
	hud->SelectActorsInRectangle<AEntitySelector>(this, &AEntitySelector::GetActorsInSelectionRectangle, mStartAnchor, mEndAnchor, true, true);
	
	//This is function is completed in GetActorsInSelectionRectangle
	bSelecting = false;
}

void AEntitySelector::CancelSelection() {
	bSelecting = false;
	auto pc = GetWorld()->GetFirstPlayerController();
	if (!pc) {
		return;
	}
	auto hud = Cast<AV_HUD>(pc->GetHUD());
	if (!hud) {
		return;
	}
	//Stop drawing rectangle
	hud->StopActorSelection();
}

void AEntitySelector::AddActorToSelection(AActor* actor) {
	
	if (IsValid(actor) && !mSelectedActors.Contains(actor)) {
		mSelectedActors.Add(actor);
	}
}

void AEntitySelector::GetActorsInSelectionRectangle(TArray<AActor*> outActors)
{
	//Get the actor beanth mouse cursor just in case
	AddActorToSelection(GetActorUnderCursor());

	//Iterate of all the found actors and add to selected, skip duplicates
	for (auto actor : outActors) {
		AddActorToSelection(actor);
	}

	//selected actors
	auto actors = mSelectedActors;

	//For general



	//Specifcally for selection groups
	for (auto& group : mSelectionGroups) {
		
		auto filter = group->mFilterClass;//Get filter from the group
		auto filtered = FilterActors(actors, filter);//Sort actors 
		group->mFoundActors = filtered;//Set the found actors
		auto& delegate = group->mCompletedDelegate;
		group->mChangedDelegate.Broadcast(*group);//Broadcast the selection group
	}
}

void AEntitySelector::CreateSelctionRequest(const FSelectionGroup& request, UObject* UserObject, FName OutFuncName) {
	auto group = MakeShared<FSelectionGroup>(request);
	group->mChangedDelegate.AddUFunction(UserObject, OutFuncName);
	mSelectionGroups.Add(group);

}

void AEntitySelector::AddActorToSelection(TSharedPtr<FSelectionGroup> selection, AActor* actor)
{
	auto filter = selection->mFilterClass;
	auto classTo = AEntitySelector::StaticClass();
	auto filtered = actor->IsA(filter);
}
TArray<AActor*>  AEntitySelector::FilterActors(TArray<AActor*> actors, UClass* filter) {
	TArray<AActor*> filtered;

	for (auto actor : actors) {
		if (actor->IsA(filter)) {
			filtered.Add(actor);
		}
	}

	return filtered;
}

TEnumAsByte<ESelectionType> AEntitySelector::SelectionType() {
	return mAddativeAxis > 0.5 ? ADDATIVE : REFRESH;
}

void AEntitySelector::TestSelectedActor(AActor* actor)
{
}

#pragma region HELPERS
FVector2D AEntitySelector::GetMousePosition() {
	auto pc = GetWorld()->GetFirstPlayerController();
	if (!pc) {
		return FVector2D(0, 0);
	}
	double MouseX = 0;
	double MouseY = 0;
	pc->GetMousePosition(MouseX, MouseY);
	return FVector2D(MouseX, MouseY);
}

AActor* AEntitySelector::GetActorUnderCursor() {
	//GetMousePosition + DeprojectScreenToWorld + LineTraceSingleByObjectType.
	auto world = GetWorld();
	if (!world) {
		return nullptr;
	}
	auto pc = world->GetFirstPlayerController();
	if (!pc) {
		return nullptr;
	}
	
	FHitResult Hit;
	pc->GetHitResultUnderCursor(ECC_Visibility, true, Hit);

	if (!Hit.bBlockingHit && !IsValid(Hit.GetActor()))
	{
		return nullptr;
	}

	auto actor = Hit.GetActor();

	return actor;
};	
#pragma endregion