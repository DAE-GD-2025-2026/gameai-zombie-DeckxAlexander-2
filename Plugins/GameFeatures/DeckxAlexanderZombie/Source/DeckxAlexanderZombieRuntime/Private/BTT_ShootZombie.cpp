// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_ShootZombie.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Common/InventoryComponent.h"
#include "DeckxAlexanderZombieRuntime/StudentPerceptor.h"
#include "Items/BaseItem.h"
#include "Kismet/KismetMathLibrary.h"


UBTT_ShootZombie::UBTT_ShootZombie()
{
}

EBTNodeResult::Type UBTT_ShootZombie::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)return EBTNodeResult::Failed;
	APawn* Survivor = Cast<APawn>(AIController->GetPawn());
	m_OwnerPawn = Survivor;
	if (!Survivor)return EBTNodeResult::Failed;
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return  EBTNodeResult::Failed;
	UStudentPerceptor* perceptor = Survivor->FindComponentByClass<UStudentPerceptor>();
	if (!perceptor) return EBTNodeResult::Failed;
	
	auto closest = GetClosestZombie(perceptor->GetSeenZombies());
	if (!closest)
	{
		BB->SetValueAsBool(FName("zombieClose"), false);
		return EBTNodeResult::Failed;
	}
	UInventoryComponent* inventory = Survivor->FindComponentByClass<UInventoryComponent>();
	auto weapon = GetWeaponSlot(inventory);
	if (weapon == -1)
	{
		BB->SetValueAsBool(FName("hasWeapon"), false);
		return EBTNodeResult::Failed;
	}
	
	TurnTowardsZombie(closest);
	inventory->UseItem(weapon);
	
	perceptor->SetZombieKilled(closest);
	
	if (inventory->GetInventory()[weapon]->GetValue() <= 0)
	{
		inventory->RemoveItem(weapon);
	}
	

	return EBTNodeResult::Succeeded;
}

int UBTT_ShootZombie::GetWeaponSlot(UInventoryComponent* inventory)
{
	int weaponSlot = -1;
	for (int slot{}; slot < inventory->GetInventory().Num(); slot++)
	{
		auto item = inventory->GetInventory()[slot];
		if (!item) continue;
		if (item->GetItemType() == EItemType::Shotgun && item->GetValue() > 0)
		{
			weaponSlot = slot;
			continue;
		}
		if (item->GetItemType() == EItemType::Pistol && item->GetValue() > 0)
		{
			weaponSlot = slot;
			break;
		}
	}
	return weaponSlot;
	
}

AActor* UBTT_ShootZombie::GetClosestZombie(TArray<AActor*> zombies)
{
	AActor* result = nullptr;
	
	if (zombies.IsEmpty()) return result;
	
	for (auto z : zombies)
	{
		if (!IsValid(z)) continue;
		
		if (!result)
		{
			result = z;
			continue;
		}
		
		if (FVector::DistSquared(z->GetActorLocation(), m_OwnerPawn->GetActorLocation()) <
			FVector::DistSquared(result->GetActorLocation(), m_OwnerPawn->GetActorLocation())) result = z;
		
	}
	
	if (FVector::DistSquared(result->GetActorLocation(), m_OwnerPawn->GetActorLocation()) > (600*600)) result = nullptr;
	
	return result;
}

void UBTT_ShootZombie::TurnTowardsZombie(AActor* target)
{
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(m_OwnerPawn->GetActorLocation(),target->GetActorLocation());

	m_OwnerPawn->SetActorRotation(TargetRotation);
}
