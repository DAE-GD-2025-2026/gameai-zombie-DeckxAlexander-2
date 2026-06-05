// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_CanFindWeapon.generated.h"

/**
 * 
 */
UCLASS()
class DECKXALEXANDERZOMBIERUNTIME_API UBTT_CanFindWeapon : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_CanFindWeapon();

protected:
	
	virtual EBTNodeResult::Type ExecuteTask(
	UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) override;
};
