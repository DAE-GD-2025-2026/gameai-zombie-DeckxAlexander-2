// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_BaseSteeringDeckxAlexander.generated.h"

/**
 * 
 */
class UBlackboardComponent;
UCLASS()
class DECKXALEXANDERZOMBIERUNTIME_API UBTT_BaseSteeringDeckxAlexander : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_BaseSteeringDeckxAlexander();
protected:
	virtual EBTNodeResult::Type ExecuteTask(
		UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory) override;

	virtual void TickTask(
		UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory,
		float DeltaSeconds) override;
	
	void RotateTowardsMovement(APawn* Pawn, FVector movementdir,  float DeltaTime);
	FVector CalculateObstacleAvoidance();
	FVector PurgeAvoidance();
	
	
	virtual FVector CalculateDesiredVelocity() {return FVector::ZeroVector;}
	
	APawn* m_OwnerPawn{};
	UBlackboardComponent* m_BlackboardComponent{};
	
private:
	FVector CalculateAvoidanceInDirection(const FVector& position, const FVector& direction, float lookAheadDistance, float weight);
	
	FCollisionQueryParams m_CollisionQueryParams;
	

};
