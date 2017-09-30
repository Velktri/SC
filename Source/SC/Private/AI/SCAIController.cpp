// Fill out your copyright notice in the Description page of Project Settings.

#include "SCAIController.h"
#include "SCSelectable.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"


ASCAIController::ASCAIController()
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoardComp"));
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
}

void ASCAIController::Possess(class APawn* InPawn)
{
	Super::Possess(InPawn);

	ASCSelectable* Bot = Cast<ASCSelectable>(InPawn);

	// start behavior
	if (Bot && Bot->GetBotBehavior())
	{
		if (Bot->GetBotBehavior()->BlackboardAsset)
		{
			BlackboardComp->InitializeBlackboard(*Bot->GetBotBehavior()->BlackboardAsset);
		}

		MovementKeyID = BlackboardComp->GetKeyID("MoveLocation");

		BehaviorComp->StartTree(*(Bot->GetBotBehavior()));
	}
}

void ASCAIController::Move(FVector Location)
{
	BlackboardComp->SetValue<UBlackboardKeyType_Vector>(MovementKeyID, Location);
}
