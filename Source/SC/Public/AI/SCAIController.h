// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SCAIController.generated.h"

/**
 * 
 */
UCLASS()
class SC_API ASCAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ASCAIController();

	virtual void Possess(class APawn* InPawn) override;

	/* Move to target point */
	UFUNCTION(BlueprintCallable, Category = Behavior)
	void Move(FVector Location);

protected:
	UPROPERTY(transient)
	UBlackboardComponent* BlackboardComp;

	/* Cached BT component */
	UPROPERTY(transient)
	class UBehaviorTreeComponent* BehaviorComp;

	int32 MovementKeyID;

private:
	/** Returns BlackboardComp subobject **/
	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return BlackboardComp; }

	/** Returns BehaviorComp subobject **/
	FORCEINLINE UBehaviorTreeComponent* GetBehaviorComp() const { return BehaviorComp; }
};
