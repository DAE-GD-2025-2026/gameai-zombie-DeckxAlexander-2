// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_ShootZombieDeckxAlexander.generated.h"

class UInventoryComponent;
class UStudentPerceptorDeckxAlexander;
/**
 * 
 */
UCLASS()
class DECKXALEXANDERZOMBIERUNTIME_API UBTT_ShootZombieDeckxAlexander : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_ShootZombieDeckxAlexander();


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
	UStudentPerceptorDeckxAlexander* m_Perceptor{};
	
	FCollisionObjectQueryParams m_ObjectParams;
\
	
	
	int m_WeaponSlot{};
	bool m_WeaponIsShotgun{false};
};
