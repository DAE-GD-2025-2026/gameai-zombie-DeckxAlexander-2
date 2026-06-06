// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_FindHouseExitDeckxAlexander.generated.h"

/**
 * 
 */
UCLASS()
class DECKXALEXANDERZOMBIERUNTIME_API UBTT_FindHouseExitDeckxAlexander : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_FindHouseExitDeckxAlexander();

protected:
	
	virtual EBTNodeResult::Type ExecuteTask(
	UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) override;
	
	FVector GetRandomPointOutsideBox(const FBox& Box, float MaxDistance);
};


