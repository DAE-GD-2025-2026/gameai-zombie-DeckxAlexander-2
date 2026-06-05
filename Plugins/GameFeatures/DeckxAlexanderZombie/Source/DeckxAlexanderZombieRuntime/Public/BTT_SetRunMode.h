// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_SetRunMode.generated.h"

/**
 * 
 */
UCLASS()
class DECKXALEXANDERZOMBIERUNTIME_API UBTT_SetRunMode : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_SetRunMode();
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	bool bRun{};

protected:
	
	virtual EBTNodeResult::Type ExecuteTask(
	UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) override;
};
