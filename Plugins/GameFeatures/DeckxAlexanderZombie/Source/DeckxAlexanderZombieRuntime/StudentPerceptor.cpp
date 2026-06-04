// Fill out your copyright notice in the Description page of Project Settings.


#include "StudentPerceptor.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Items/BaseItem.h"
#include "Zombies/BaseZombie.h"
#include "Village/House/House.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISense_Damage.h"


UStudentPerceptor::UStudentPerceptor()
{
	PrimaryComponentTick.bCanEverTick = true;
	

}

void UStudentPerceptor::BeginPlay()
{
	Super::BeginPlay();
	
	if (auto PerceptionComp = GetOwner()->GetComponentByClass<UAIPerceptionComponent>())
	{
		PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &UStudentPerceptor::OnPerceptionUpdated);
	}
	

}

void UStudentPerceptor::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	GEngine->AddOnScreenDebugMessage(5, 1.f, FColor::Green, 
	FString::Printf(TEXT("Saw Something!")));
	
	
	if (Actor->IsA(AHouse::StaticClass()))
	{
		AddHouseToMemory(Actor);
	}
	
	if (Actor->IsA(ABaseItem::StaticClass()))
	{
		AddItemToSeen(Actor);
	}
	
	if (Actor->IsA(ABaseZombie::StaticClass()))
	{
		AddZombieToMemory(Actor);
	}
	
	
}

void UStudentPerceptor::AddItemToSeen(AActor* Actor)
{
	if (m_SeenItems.Contains(Actor)) return;
	m_SeenItems.Add(Actor);
	
	//Sort by range
	FVector actorloc = GetOwner()->GetActorLocation();
	
	m_SeenItems.Sort([&](const AActor& A, const AActor& B)
	{
	return FVector::DistSquared(A.GetActorLocation(), actorloc)
		 < FVector::DistSquared(B.GetActorLocation(), actorloc);
	});
	
}

void UStudentPerceptor::SetItemInMemory(AActor* Actor)
{
	m_SeenItems.Remove(Actor);
	if (m_ItemsInMemory.Contains(Actor)) return;
	m_ItemsInMemory.Add(Actor);
	
	FVector actorloc = GetOwner()->GetActorLocation();
	m_ItemsInMemory.Sort([&](const AActor& A, const AActor& B)
	{
	return FVector::DistSquared(A.GetActorLocation(), actorloc)
	 < FVector::DistSquared(B.GetActorLocation(), actorloc);
	});
}

void UStudentPerceptor::AddHouseToMemory(AActor* Actor)
{
	if (m_ExploredHouses.Contains(Actor) || m_SeenUnExploredHouses.Contains(Actor)) return;
	
	m_SeenUnExploredHouses.Add(Actor);
	
	
	//Sort by range
	FVector actorloc = GetOwner()->GetActorLocation();
	
	m_SeenUnExploredHouses.Sort([&](const AActor& A, const AActor& B)
	{
	return FVector::DistSquared(A.GetActorLocation(), actorloc)
		 < FVector::DistSquared(B.GetActorLocation(), actorloc);
	});
	
}

bool UStudentPerceptor::CheckIfInHouse()
{
	if (!m_CurrentHouse)
	{
		for (AActor* actor : m_SeenUnExploredHouses)
		{
			auto house = Cast<AHouse>(actor);
			if (!house) continue;
			
			const FHouseBounds Bounds = house->GetBounds();
			FBox Box(Bounds.Origin - Bounds.Extent,Bounds.Origin + Bounds.Extent);

			if (FMath::PointBoxIntersection(GetOwner()->GetActorLocation(), Box))
			{
				m_CurrentHouse = actor;
				SetHouseAsExplored(m_CurrentHouse);
				return true;
			}
		}
		
		for (AActor* actor : m_ExploredHouses)
		{
			auto house = Cast<AHouse>(actor);
			if (!house) continue;
			
			const FHouseBounds Bounds = house->GetBounds();
			FBox Box(Bounds.Origin - Bounds.Extent,Bounds.Origin + Bounds.Extent);

			if (FMath::PointBoxIntersection(GetOwner()->GetActorLocation(), Box))
			{
				m_CurrentHouse = actor;
				SetHouseAsExplored(m_CurrentHouse);
				return true;
			}
		}
	}
	else
	{
		auto house = Cast<AHouse>(m_CurrentHouse);
		const FHouseBounds Bounds = house->GetBounds();
		FBox Box(Bounds.Origin - Bounds.Extent,Bounds.Origin + Bounds.Extent);
		if (FMath::PointBoxIntersection(GetOwner()->GetActorLocation(), Box))
		{
			return true;
		}
	}
	
	
	m_CurrentHouse = nullptr;
	return false;
	
	
}

void UStudentPerceptor::SetZombieKilled(AActor* Actor)
{
	if (m_KilledZombies.Contains(Actor)) return;
	m_SeenZombies.Remove(Actor);
	m_KilledZombies.Add(Actor);
}

void UStudentPerceptor::RemoveItemFromMemory(AActor* Actor)
{
	m_SeenItems.Remove(Actor);
	m_ItemsInMemory.Remove(Actor);
}

void UStudentPerceptor::SetHouseAsExplored(AActor* Actor)
{
	m_ExploredHouses.Add(Actor);
	m_SeenUnExploredHouses.Remove(Actor);
	
}

void UStudentPerceptor::AddZombieToMemory(AActor* Actor)
{
	if (m_SeenZombies.Contains(Actor)) return;
	
	m_SeenZombies.Add(Actor);
	
	
}


