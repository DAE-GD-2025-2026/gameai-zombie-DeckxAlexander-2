// Fill out your copyright notice in the Description page of Project Settings.

#include "AIController.h"
#include "BTT_SteeringMoveTo.h"

#include <string>

#include "BehaviorTree/BlackboardComponent.h"
#include "DeckxAlexanderZombieRuntime/StudentPerceptor.h"
#include "Survivor/SurvivorPawn.h"


UBTT_SteeringMoveTo::UBTT_SteeringMoveTo()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_SteeringMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	m_OwnerPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (!m_OwnerPawn) return EBTNodeResult::Failed;

	
	m_CurrentIndex = 0;
	
	auto survivorPawn = Cast<ASurvivorPawn>(m_OwnerPawn);
	if (!survivorPawn) return EBTNodeResult::Failed;
	
	m_BlackboardComponent = OwnerComp.GetAIOwner()->GetBlackboardComponent();
	if (!m_BlackboardComponent) return EBTNodeResult::Failed;
	
	m_Path = survivorPawn->CalculatePath(m_BlackboardComponent->GetValueAsVector(FName("TargetLocation")));
	
	if (m_Path.IsEmpty()) return EBTNodeResult::Succeeded;
	
	m_CurrentTargetLocation =m_Path[m_CurrentIndex];
	
	return EBTNodeResult::InProgress;
}

void UBTT_SteeringMoveTo::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	FVector movementdir=CalculateDesiredVelocity();
	m_OwnerPawn->AddMovementInput(movementdir);
	RotateTowardsMovement(m_OwnerPawn, movementdir, DeltaSeconds);
	
	GEngine->AddOnScreenDebugMessage(
		5,
		1.f,
		FColor::Orange,
		FString::Printf(TEXT("DistSq: %.2f"),
			FVector::DistSquared(
				m_CurrentTargetLocation,
				m_OwnerPawn->GetActorLocation()))
	);
	
	if (FVector::DistSquared(m_CurrentTargetLocation, m_OwnerPawn->GetActorLocation()) < (50*50))
	{
		m_CurrentIndex++;
		
		if (m_CurrentIndex >= m_Path.Num())
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			
			if (bIsTargetHouse)
			{
				auto perceptor = m_OwnerPawn->FindComponentByClass<UStudentPerceptor>();
				perceptor->CheckIfInHouse();	
			}
			
			return;
		}
		
		m_CurrentTargetLocation =m_Path[m_CurrentIndex];
	}
	

	
}

FVector UBTT_SteeringMoveTo::CalculateDesiredVelocity()
{
	FVector desiredVelocity = m_CurrentTargetLocation - m_OwnerPawn->GetActorLocation();
	
	GEngine->AddOnScreenDebugMessage(6, 1.f, FColor::Green, 
m_CurrentTargetLocation.ToString());
	GEngine->AddOnScreenDebugMessage(7, 1.f, FColor::Green, 
m_OwnerPawn->GetActorLocation().ToString());
	GEngine->AddOnScreenDebugMessage(8, 1.f, FColor::Orange, 
desiredVelocity.ToString());
	
	desiredVelocity.Z = 0;
	desiredVelocity.Normalize();
	
	FVector purgeAvoidance = PurgeAvoidance();
	
	FVector finalVelocity= desiredVelocity + (purgeAvoidance*5.f);
	
	return finalVelocity;
}
