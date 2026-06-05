// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_FleeFromZombie.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DeckxAlexanderZombieRuntime/StudentPerceptor.h"
#include "Survivor/SurvivorPawn.h"
#include "Zombies/BaseZombie.h"

UBTT_FleeFromZombie::UBTT_FleeFromZombie()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_FleeFromZombie::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)return EBTNodeResult::Failed;
	if (!m_OwnerPawn)return EBTNodeResult::Failed;
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return  EBTNodeResult::Failed;
	auto perceptor = m_OwnerPawn->FindComponentByClass<UStudentPerceptor>();
	if (!perceptor) return EBTNodeResult::Failed;
	

	
	m_ClosestZombie = GetClosestZombie(perceptor->GetSeenZombies());
	if (!m_ClosestZombie)
	{
		BB->SetValueAsBool(FName("ZombieClose"), false);
		return EBTNodeResult::Failed;
	}
	
	return EBTNodeResult::InProgress;
}

AActor* UBTT_FleeFromZombie::GetClosestZombie(TArray<AActor*> zombies)
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

FVector UBTT_FleeFromZombie::CalculateDesiredVelocity()
{
	FVector velocity = m_OwnerPawn->GetActorLocation() - m_ClosestZombie->GetActorLocation();
	velocity.Z = 0;
	velocity.Normalize();
	
	FVector avoidanceVelocity = CalculateObstacleAvoidance();
	FVector purgeAvoidanceVelocity = PurgeAvoidance();
	FVector finalVelocity = velocity + (avoidanceVelocity*2.f) + (purgeAvoidanceVelocity*5.f);
	
	return finalVelocity;
}
