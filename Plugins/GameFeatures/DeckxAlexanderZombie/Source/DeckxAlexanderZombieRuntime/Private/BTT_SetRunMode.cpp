// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_SetRunMode.h"
#include "AIController.h"
#include "Survivor/SurvivorPawn.h"

UBTT_SetRunMode::UBTT_SetRunMode()
{
}

EBTNodeResult::Type UBTT_SetRunMode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)return EBTNodeResult::Failed;
	ASurvivorPawn* Survivor = Cast<ASurvivorPawn>(AIController->GetPawn());
	if (!Survivor)return EBTNodeResult::Failed;
	
	if (bRun) Survivor->StartRunning();
	else Survivor->StopRunning();

	return EBTNodeResult::Succeeded;
}
