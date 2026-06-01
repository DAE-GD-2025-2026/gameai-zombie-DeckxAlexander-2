// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_BaseSteering.generated.h"

/**
 * 
 */
UCLASS()
class DECKXALEXANDERZOMBIERUNTIME_API UBTT_BaseSteering : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_BaseSteering();
protected:
	virtual EBTNodeResult::Type ExecuteTask(
		UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory) override;

	virtual void TickTask(
		UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory,
		float DeltaSeconds) override;
	
	void RotateTowardsMovement(APawn* Pawn, FVector movementdir,  float DeltaTime);
	
	virtual FVector CalculateDesiredVelocity() {return FVector::ZeroVector;}
	
	APawn* m_OwnerPawn;
};
