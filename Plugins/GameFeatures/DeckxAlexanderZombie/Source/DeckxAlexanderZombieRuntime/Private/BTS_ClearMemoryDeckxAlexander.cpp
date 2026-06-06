// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_ClearMemoryDeckxAlexander.h"
#include "DeckxAlexanderZombieRuntime/StudentPerceptorDeckxAlexander.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"


UBTS_ClearMemoryDeckxAlexander::UBTS_ClearMemoryDeckxAlexander()
{
	NodeName = TEXT("Clear Survivor Memory");

	Interval = 20.f;
	RandomDeviation = 1.f;
}



void UBTS_ClearMemoryDeckxAlexander::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	if (!m_Perceptor)
	{
		AAIController* AIController = OwnerComp.GetAIOwner();
		if (!AIController)return;
		APawn* Survivor = Cast<APawn>(AIController->GetPawn());
		if (!Survivor)return;
		UBlackboardComponent* BB =OwnerComp.GetBlackboardComponent();
		if (!BB) return;
		m_Perceptor = Survivor->FindComponentByClass<UStudentPerceptorDeckxAlexander>();
		if (m_Perceptor) return;
	};
	
	m_Perceptor->ClearItemsInMemory();
	m_Perceptor->ClearHousesInMemory();
	
}
