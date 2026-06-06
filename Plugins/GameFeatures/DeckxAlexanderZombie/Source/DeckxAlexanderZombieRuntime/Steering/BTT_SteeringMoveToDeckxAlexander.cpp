// Fill out your copyright notice in the Description page of Project Settings.

#include "AIController.h"
#include "BTT_SteeringMoveToDeckxAlexander.h"

#include <string>

#include "BehaviorTree/BlackboardComponent.h"
#include "DeckxAlexanderZombieRuntime/StudentPerceptorDeckxAlexander.h"
#include "Survivor/SurvivorPawn.h"


UBTT_SteeringMoveToDeckxAlexander::UBTT_SteeringMoveToDeckxAlexander()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_SteeringMoveToDeckxAlexander::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

void UBTT_SteeringMoveToDeckxAlexander::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	FVector movementdir=CalculateDesiredVelocity();
	m_OwnerPawn->AddMovementInput(movementdir);
	RotateTowardsMovement(m_OwnerPawn, movementdir, DeltaSeconds);
	
	DrawPath(m_Path, GetWorld());
	
	if (FVector::DistSquared(m_CurrentTargetLocation, m_OwnerPawn->GetActorLocation()) < (50*50))
	{
		m_CurrentIndex++;
		
		if (m_CurrentIndex >= m_Path.Num())
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			
			if (bIsTargetHouse)
			{
				auto perceptor = m_OwnerPawn->FindComponentByClass<UStudentPerceptorDeckxAlexander>();
				perceptor->CheckIfInHouse();	
			}
			
			return;
		}
		
		m_CurrentTargetLocation =m_Path[m_CurrentIndex];
	}
	

	
}

FVector UBTT_SteeringMoveToDeckxAlexander::CalculateDesiredVelocity()
{
	FVector desiredVelocity = m_CurrentTargetLocation - m_OwnerPawn->GetActorLocation();
	
	
	desiredVelocity.Z = 0;
	desiredVelocity.Normalize();
	
	FVector purgeAvoidance = PurgeAvoidance();
	
	FVector finalVelocity= desiredVelocity + (purgeAvoidance);
	
	return finalVelocity;
}

void UBTT_SteeringMoveToDeckxAlexander::DrawPath(const TArray<FVector>& Points, UWorld* World)
{
	if (Points.Num() < 2 || !World)
	{
		return;
	}

	for (int32 i = 0; i < Points.Num() - 1; ++i)
	{
		DrawDebugLine(World,Points[i],Points[i + 1],FColor::Green,false,-1.0f,0,2.0f);
	}
}
