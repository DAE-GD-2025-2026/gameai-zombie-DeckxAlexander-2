// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_ShootZombie.generated.h"

class UInventoryComponent;
class UStudentPerceptor;
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
	
	virtual void TickTask(
	UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory,
	float DeltaSeconds) override;
	
	int GetWeaponSlot(UInventoryComponent* inventory);
	AActor* GetClosestZombie(TArray<AActor*> zombies);
	
	bool AimTowardsZombie(AActor* target, float deltaTime);
	
private:
	APawn* m_OwnerPawn{};
	
	AActor* m_ClosestZombie{};
	UInventoryComponent* m_Inventory{};
	UStudentPerceptor* m_Perceptor{};
	
	
	int m_WeaponSlot{};
	bool m_WeaponIsShotgun{false};
};
