// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_BaseSteering.h"
#include "AIController.h"

UBTT_BaseSteering::UBTT_BaseSteering()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_BaseSteering::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	m_OwnerPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	return EBTNodeResult::InProgress;
}

void UBTT_BaseSteering::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (m_OwnerPawn == nullptr) return;
	
	FVector movementdir = CalculateDesiredVelocity();
	m_OwnerPawn->AddMovementInput(movementdir);
	RotateTowardsMovement(m_OwnerPawn, movementdir, DeltaSeconds);
	
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}

void UBTT_BaseSteering::RotateTowardsMovement(APawn* Pawn, FVector movementdir, float DeltaTime)
{
	if (!Pawn) return;
	
	const FRotator currentRot = Pawn->GetActorRotation();
	const FRotator movementRot = movementdir.Rotation();

	const FRotator newRot =FMath::RInterpTo(currentRot,movementRot,DeltaTime,5.f);

	Pawn->SetActorRotation(newRot);
}
