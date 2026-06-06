// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Items/BaseItem.h"
#include "BTT_SearchForItemInMemoryDeckxAlexander.generated.h"

/**
 * 
 */
UCLASS()
class DECKXALEXANDERZOMBIERUNTIME_API UBTT_SearchForItemInMemoryDeckxAlexander : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_SearchForItemInMemoryDeckxAlexander();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<EItemType> ItemType{};

protected:
	
	virtual EBTNodeResult::Type ExecuteTask(
	UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) override;
};
