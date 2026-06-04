// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_ShootZombie.generated.h"

class UInventoryComponent;
/**
 * 
 */
UCLASS()
class DECKXALEXANDERZOMBIERUNTIME_API UBTT_ShootZombie : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_ShootZombie();


protected:
	
	virtual EBTNodeResult::Type ExecuteTask(
	UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) override;
	
	int GetWeaponSlot(UInventoryComponent* inventory);
	AActor* GetClosestZombie(TArray<AActor*> zombies);
	
	void TurnTowardsZombie(AActor* target);
	
private:
	APawn* m_OwnerPawn{};
};
