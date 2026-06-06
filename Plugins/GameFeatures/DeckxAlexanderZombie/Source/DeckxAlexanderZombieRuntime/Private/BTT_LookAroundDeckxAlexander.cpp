// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_LookAroundDeckxAlexander.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DeckxAlexanderZombieRuntime/StudentPerceptorDeckxAlexander.h"

UBTT_LookAroundDeckxAlexander::UBTT_LookAroundDeckxAlexander()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_LookAroundDeckxAlexander::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)return EBTNodeResult::Failed;
	APawn* Survivor = Cast<APawn>(AIController->GetPawn());
	m_OwnerPawn = Survivor;
	if (!Survivor)return EBTNodeResult::Failed;
	m_Blackboard = OwnerComp.GetBlackboardComponent();
	if (!m_Blackboard) return  EBTNodeResult::Failed;
	m_Perceptor = Survivor->FindComponentByClass<UStudentPerceptorDeckxAlexander>();
	if (!m_Perceptor) return EBTNodeResult::Failed;
	
	m_Blackboard->SetValueAsBool(FName("LookAround"), false);
	
	m_AccumulatedYaw = 0.f;
	m_TargetYaw = 360.f;
	
	return EBTNodeResult::InProgress;
}

void UBTT_LookAroundDeckxAlexander::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	const float Speed = 720.f; // degrees/sec
	float Step = Speed * DeltaSeconds;

	m_AccumulatedYaw += Step;

	FRotator Rot = m_OwnerPawn->GetActorRotation();
	Rot.Yaw += Step;
	m_OwnerPawn->SetActorRotation(Rot);
	

	if (m_Perceptor->GetSeenZombies().Num() > 0)
	{
		m_Blackboard->SetValueAsBool(FName("ZombieClose"), true);
	}
	

	if (m_AccumulatedYaw >= m_TargetYaw)
	{
		Rot.Yaw = FMath::UnwindDegrees(Rot.Yaw);
		m_OwnerPawn->SetActorRotation(Rot);

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	
}
