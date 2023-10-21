// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EntitySelector.generated.h"



DECLARE_MULTICAST_DELEGATE_OneParam(FOnRequestChanged, FSelectionGroup);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnRequestCompleted, FSelectionGroup);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActorSelectedSignature, AActor*, _actor);
USTRUCT(BlueprintType)
struct FSelectionGroup {
	GENERATED_BODY()
public:
	FSelectionGroup() {
		mGUID = FGuid::NewGuid();
	}
	FSelectionGroup(FString name, FString description, UClass* filterClass) {
		mGUID = FGuid::NewGuid();
		mName = name;
		mDescription = description;
		mFilterClass = filterClass;

		//Create delegate
		mChangedDelegate = FOnRequestChanged();
	}
	~FSelectionGroup() {
		//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, FString::Printf(TEXT("SG : Destructed %s"), *mName));
	}

	FGuid mGUID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString mName = "Group";
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString mDescription = "Description";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UClass* mFilterClass;//What actors to find, for example ADog

	//DELEGATES
	FOnRequestChanged mChangedDelegate;//Delegate called on chagnes
	FOnRequestCompleted mCompletedDelegate;//Delegate called on completion

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AActor*> mFoundActors;
	//bool operator ==(const FSelectionGroup& g) {
	//	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, FString::Printf(TEXT("Comparing %s and %s"), *this->mName, *g.mName));
	//	return this->mGUID == g.mGUID;
	//}
};

UENUM()
enum ESelectionType { ADDATIVE, REFRESH };

UCLASS()
class VERGJORN_API AEntitySelector : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEntitySelector();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	bool bSelecting = false;//Is selecting
	FVector2D mStartAnchor;//Where did selection start
	FVector2D mEndAnchor;//Where did selection end

	void SetAddative(float value);
	float mAddativeAxis = 0;
	void StartSelection();
	void WhileSelection();
	void StopSelection();
	void CancelSelection();

	void AddActorToSelection(AActor* actor);


	void GetActorsInSelectionRectangle(TArray<AActor*> outActors);//Function called once the selection has been made

	UFUNCTION(BlueprintCallable)
		void CreateSelctionRequest(const FSelectionGroup& request, UObject* UserObject, FName OutFuncName);
	template<typename T>
	void CreateSelectionRequest(const FSelectionGroup& request, T* UserObject, void(T::*ChangedFunc)(FSelectionGroup));//Call this to create a selection request
	

	//TESTING
	UFUNCTION()
	void TestingChangedOrCompleted(FSelectionGroup group) {

		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString::Printf(TEXT("Testing Changed or Compelted : Got Selction : %s"), *group.mName));

	};

	void AddActorToSelection(TSharedPtr<FSelectionGroup> selection, AActor* actor);

	FSelectionGroup mActiveSelection;
	TArray<TSharedRef<FSelectionGroup>> mSelectionGroups;

	TArray<AActor*> mSelectedActors;
	TArray<AActor*> FilterActors(TArray<AActor*> actors, UClass* filter);

	TEnumAsByte<ESelectionType> mSelectionType;
	UFUNCTION(BlueprintCallable)
	TEnumAsByte<ESelectionType> SelectionType();
	UPROPERTY(BlueprintAssignable)
		FOnActorSelectedSignature mOnActorSelected;

	void TestSelectedActor(AActor* actor);

	//HELPERS
	FVector2D GetMousePosition();//Mouse screen position

	AActor* GetActorUnderCursor();//Actor under cursor ECC_Visibilty

};

template<typename T>
void AEntitySelector::CreateSelectionRequest(const FSelectionGroup& request, T* UserObject, void(T::* ChangedFunc)(FSelectionGroup)) {
	auto group = MakeShared<FSelectionGroup>(request);
	group->mChangedDelegate.AddUObject(UserObject, ChangedFunc);
	mSelectionGroups.Add(group);

}
