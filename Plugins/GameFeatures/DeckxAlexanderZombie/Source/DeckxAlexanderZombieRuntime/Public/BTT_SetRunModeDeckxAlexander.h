// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_SetRunModeDeckxAlexander.generated.h"

/**
 * 
 */
UCLASS()
class DECKXALEXANDERZOMBIERUNTIME_API UBTT_SetRunModeDeckxAlexander : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_SetRunModeDeckxAlexander();
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	bool bRun{};

protected:
	
	virtual EBTNodeResult::Type ExecuteTask(
	UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) override;
};
