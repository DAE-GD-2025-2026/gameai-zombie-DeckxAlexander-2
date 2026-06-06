// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_ShootZombieDeckxAlexander.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Common/InventoryComponent.h"
#include "DeckxAlexanderZombieRuntime/StudentPerceptorDeckxAlexander.h"
#include "Items/BaseItem.h"
#include "Kismet/KismetMathLibrary.h"
#include "Zombies/BaseZombie.h"


UBTT_ShootZombieDeckxAlexander::UBTT_ShootZombieDeckxAlexander()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_ShootZombieDeckxAlexander::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)return EBTNodeResult::Failed;
	APawn* Survivor = Cast<APawn>(AIController->GetPawn());
	m_OwnerPawn = Survivor;
	if (!Survivor)return EBTNodeResult::Failed;
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return  EBTNodeResult::Failed;
	m_Perceptor = Survivor->FindComponentByClass<UStudentPerceptorDeckxAlexander>();
	if (!m_Perceptor) return EBTNodeResult::Failed;
	
	m_ClosestZombie = GetClosestZombie(m_Perceptor->GetSeenZombies());
	if (!m_ClosestZombie)
	{
		BB->SetValueAsBool(FName("ZombieClose"), false);
		return EBTNodeResult::Failed;
	}
	
	m_Inventory = m_OwnerPawn->FindComponentByClass<UInventoryComponent>();
	m_WeaponSlot = GetWeaponSlot(m_Inventory);
	if (m_WeaponSlot == -1)
	{
		BB->SetValueAsBool(FName("hasWeapon"), false);
		return EBTNodeResult::Failed;
	}
	
	m_ObjectParams.AddObjectTypesToQuery(ECC_WorldStatic);

	return EBTNodeResult::InProgress;
}

void UBTT_ShootZombieDeckxAlexander::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	if (!m_ClosestZombie || IsValid(m_ClosestZombie) )
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	
	if ( AimTowardsZombie(m_ClosestZombie, DeltaSeconds))
	{
		m_Inventory->UseItem(m_WeaponSlot);
	
		GEngine->AddOnScreenDebugMessage(INDEX_NONE,5.f,FColor::Red,"Shot Fired!", true);
		
		if (m_Inventory->GetInventory()[m_WeaponSlot]->GetValue() <= 0)
		{
			m_Inventory->RemoveItem(m_WeaponSlot);
		}
		
		if (IsValid(m_ClosestZombie))
		{
			return;
		}
		
		m_Perceptor->SetZombieKilled(m_ClosestZombie);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

int UBTT_ShootZombieDeckxAlexander::GetWeaponSlot(UInventoryComponent* inventory)
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

AActor* UBTT_ShootZombieDeckxAlexander::GetClosestZombie(TArray<AActor*> zombies)
{
	AActor* result = nullptr;
	
	if (zombies.IsEmpty()) return result;
	
	for (auto z : zombies)
	{
		if (!z || !IsValid(z)) continue;
		
		if (!result)
		{
			result = z;
			continue;
		}
		
		if (FVector::DistSquared(z->GetActorLocation(), m_OwnerPawn->GetActorLocation()) <
			FVector::DistSquared(result->GetActorLocation(), m_OwnerPawn->GetActorLocation())) result = z;
		
	}
	
	if (result && FVector::DistSquared(result->GetActorLocation(), m_OwnerPawn->GetActorLocation()) > (600*600)) result = nullptr;
	
	return result;
}

bool UBTT_ShootZombieDeckxAlexander::AimTowardsZombie(AActor* target, float deltaTime)
{
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(m_OwnerPawn->GetActorLocation(),target->GetActorLocation());
	
	TargetRotation.Pitch = 0.f;
	TargetRotation.Roll = 0.f;
	
	FRotator NewRotation = FMath::RInterpTo(
		m_OwnerPawn->GetActorRotation(),
		TargetRotation,
		deltaTime,
		12.f);

	m_OwnerPawn->SetActorRotation(NewRotation);

	float YawDifference = FMath::Abs(
		FMath::FindDeltaAngleDegrees(
			NewRotation.Yaw,
			TargetRotation.Yaw));

	FHitResult hitResult;
	const bool bHit = GetWorld()->LineTraceSingleByObjectType(hitResult,m_OwnerPawn->GetActorLocation(),target->GetActorLocation(),m_ObjectParams);
	
	DrawDebugLine(GetWorld(),m_OwnerPawn->GetActorLocation(),target->GetActorLocation(),bHit ? FColor::Red : FColor::Green,false,0.f);
	
	if (bHit)
	{
		DrawDebugSphere(GetWorld(),hitResult.Location,25.f,12,FColor::Red ,false,0.f);
		return false;
	}
	
	if (YawDifference < 2.0f) 
	{
		return true;
	}
	
	return false;
}
