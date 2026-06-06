// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTT_BaseSteeringDeckxAlexander.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_FleeFromZombieDeckxAlexander.generated.h"

/**
 * 
 */
UCLASS()
class DECKXALEXANDERZOMBIERUNTIME_API UBTT_FleeFromZombieDeckxAlexander : public UBTT_BaseSteeringDeckxAlexander
{
	GENERATED_BODY()
public:
	UBTT_FleeFromZombieDeckxAlexander();


protected:
	virtual EBTNodeResult::Type ExecuteTask(
	UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) override;
	
	AActor* GetClosestZombie(TArray<AActor*> zombies);
	virtual FVector CalculateDesiredVelocity() override;
	
private:
	AActor* m_ClosestZombie{};
};
