#include "VergTreeNode.h"
#include "Components/StaticMeshComponent.h"
#include "EconomySubsystem.h"
#include "VergjornTags.h"

AVergTreeNode::AVergTreeNode()
{
	PrimaryActorTick.bCanEverTick = false;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(MeshComponent);
}

void AVergTreeNode::BeginPlay()
{
	Super::BeginPlay();
	PostChopJob();

	// Subscribe to job completion messages — check if it's our chop job
	if (UGameplayMessageSubsystem* MsgSys = UGameplayMessageSubsystem::GetIfValid(this))
	{
		MessageListenerHandle = MsgSys->RegisterListener<FJobCompletedMessage>(
			TAG_Message_Job_Completed,
			this,
			&AVergTreeNode::OnJobCompleted
		);
	}
}

void AVergTreeNode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// Cancel the job if it is still open (e.g. tree placed then map unloaded)
	if (ChopJobHandle.IsValid())
	{
		if (UJobSubsystem* Jobs = GetWorld()->GetSubsystem<UJobSubsystem>())
		{
			Jobs->CancelJob(ChopJobHandle);
		}
		ChopJobHandle.Invalidate();
	}

	if (UGameplayMessageSubsystem* MsgSys = UGameplayMessageSubsystem::GetIfValid(this))
	{
		MsgSys->UnregisterListener(MessageListenerHandle);
	}

	Super::EndPlay(EndPlayReason);
}

void AVergTreeNode::PostChopJob()
{
	UJobSubsystem* Jobs = GetWorld()->GetSubsystem<UJobSubsystem>();
	if (!Jobs) return;

	FJobRecord Job;
	Job.JobType            = TAG_Job_Chop;
	Job.RequiredOccupation = TAG_Occupation_Lumberjack;
	Job.Location           = GetActorLocation();
	Job.Priority           = 5;
	Job.WorkUnitsRequired  = ChopWorkUnits;
	Job.TargetActor        = this;

	ChopJobHandle = Jobs->PostJob(Job);
	UE_LOG(LogTemp, Verbose, TEXT("TreeNode: posted Job.Chop id=%d at %s"),
		ChopJobHandle.Id, *GetActorLocation().ToString());
}

void AVergTreeNode::OnJobCompleted(FGameplayTag Channel, const FJobCompletedMessage& Msg)
{
	if (Msg.Handle == ChopJobHandle)
	{
		ChopJobHandle.Invalidate();
		Chop();
	}
}

void AVergTreeNode::Chop()
{
	// Deposit wood to the global economy
	if (UEconomySubsystem* Economy = GetWorld()->GetSubsystem<UEconomySubsystem>())
	{
		Economy->Deposit(TAG_Resource_Raw_Wood, WoodYield);
		UE_LOG(LogTemp, Log, TEXT("TreeNode chopped: +%d wood"), WoodYield);
	}

	if (RespawnDelay > 0.f)
	{
		// Hide mesh; respawn a new tree here after delay
		MeshComponent->SetVisibility(false);
		FTimerHandle RespawnTimer;
		GetWorld()->GetTimerManager().SetTimer(RespawnTimer, [WeakThis = TWeakObjectPtr<AVergTreeNode>(this)]()
		{
			if (WeakThis.IsValid())
			{
				// Re-post the chop job so another lumberjack can work this spot
				WeakThis->MeshComponent->SetVisibility(true);
				WeakThis->PostChopJob();
			}
		}, RespawnDelay, false);
	}
	else
	{
		Destroy();
	}
}
