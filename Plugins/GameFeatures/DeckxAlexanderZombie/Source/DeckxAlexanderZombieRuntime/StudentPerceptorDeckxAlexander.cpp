// Fill out your copyright notice in the Description page of Project Settings.


#include "StudentPerceptorDeckxAlexander.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Items/BaseItem.h"
#include "Zombies/BaseZombie.h"
#include "Village/House/House.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISense_Damage.h"
#include "PurgeZones/PurgeZone.h"


UStudentPerceptorDeckxAlexander::UStudentPerceptorDeckxAlexander()
{
	PrimaryComponentTick.bCanEverTick = true;
	

}

void UStudentPerceptorDeckxAlexander::BeginPlay()
{
	Super::BeginPlay();
	
	if (auto PerceptionComp = GetOwner()->GetComponentByClass<UAIPerceptionComponent>())
	{
		PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &UStudentPerceptorDeckxAlexander::OnPerceptionUpdated);
	}
	

}

void UStudentPerceptorDeckxAlexander::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Actor) return;
	if (Actor->IsA(AHouse::StaticClass()))
	{
		AddHouseToMemory(Actor);
		GEngine->AddOnScreenDebugMessage(5, 1.f, FColor::Green, 
FString::Printf(TEXT("Saw HOUSE!")));
	}
	
	if (Actor->IsA(ABaseItem::StaticClass()))
	{
		AddItemToSeen(Actor);
		GEngine->AddOnScreenDebugMessage(5, 1.f, FColor::Green, 
FString::Printf(TEXT("Saw ITEM!")));
	}
	
	if (Actor->IsA(ABaseZombie::StaticClass()))
	{
		AddZombieToMemory(Actor);
		GEngine->AddOnScreenDebugMessage(5, 1.f, FColor::Green, 
FString::Printf(TEXT("Saw ZOMBIE!")));
	}
	if (Actor->IsA(APurgeZone::StaticClass()))
	{
		AddPurgeZoneToMemory(Actor);
		GEngine->AddOnScreenDebugMessage(5, 1.f, FColor::Green, 
FString::Printf(TEXT("Saw PURGE!")));
	}

}

AActor* UStudentPerceptorDeckxAlexander::GetClosestPurgeZone()
{
	if (m_PurgeZones.IsEmpty()) return nullptr;
	float finalDistance{FLT_MAX};
	AActor* outActor{nullptr};
	TArray<AActor*> garbageActors; //Incase actors are deleted
	for (AActor* actor : m_PurgeZones)
	{
		if (!actor || !IsValid(actor))
		{
			garbageActors.Add(actor);
			continue;
		}
		
		float dist = FVector::DistSquared(GetOwner()->GetActorLocation(), actor->GetActorLocation());
		if (dist < finalDistance)
		{
			finalDistance = dist;
			outActor = actor;
		}
	}
	
	for (AActor* actor : garbageActors)
	{
		m_PurgeZones.Remove(actor);
	}
	
	if (finalDistance > 2000*2000) return nullptr;
	return outActor;
}

AActor* UStudentPerceptorDeckxAlexander::GetClosestItemOfTypeInMemory(EItemType type)
{
	float maxDistance{FLT_MAX};
	AActor* chosenItem = nullptr;
	
	
	TArray<AActor*> invalidActors{};
	for (AActor* actor : m_ItemsMemory)
	{
		if (!actor || !IsValid(actor))
		{
			invalidActors.Add(actor);
			continue;
		}
		
		auto item = Cast<ABaseItem>(actor);
		if (item && IsValid(item) && item->GetItemType() == type)
		{
			float dist = FVector::DistSquared(GetOwner()->GetActorLocation(), actor->GetActorLocation());
			if (dist < maxDistance )
			{
				maxDistance = dist;
				chosenItem = actor;
			}
		}
	}
	
	for (auto invalid : invalidActors) //Cleanup Items Memory
	{
		m_ItemsMemory.Remove(invalid);
	}
	
	return chosenItem;
}

void UStudentPerceptorDeckxAlexander::AddItemToSeen(AActor* Actor)
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

void UStudentPerceptorDeckxAlexander::SetItemInMemory(AActor* Actor)
{
	m_SeenItems.Remove(Actor);
	if (m_ItemsVisited.Contains(Actor)) return;
	m_ItemsVisited.Add(Actor);
	if (m_ItemsMemory.Contains(Actor)) return;
	m_ItemsMemory.Add(Actor);
	
}

void UStudentPerceptorDeckxAlexander::AddHouseToMemory(AActor* Actor)
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

bool UStudentPerceptorDeckxAlexander::CheckIfInHouse()
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
			SetHouseAsExplored(m_CurrentHouse);
			return true;
			
		}
	}
	
	
	m_CurrentHouse = nullptr;
	return false;
	
	
}

void UStudentPerceptorDeckxAlexander::SetZombieKilled(AActor* Actor)
{
	if (m_KilledZombies.Contains(Actor)) return;
	m_SeenZombies.Remove(Actor);
	m_KilledZombies.Add(Actor);
}

void UStudentPerceptorDeckxAlexander::RemoveItemFromMemory(AActor* Actor)
{
	m_SeenItems.Remove(Actor);
	m_ItemsVisited.Remove(Actor);
	m_ItemsMemory.Remove(Actor);
}

void UStudentPerceptorDeckxAlexander::SetHouseAsExplored(AActor* Actor)
{
	m_SeenUnExploredHouses.Remove(Actor);
	if (m_ExploredHouses.Contains(Actor)) return;
	m_ExploredHouses.Add(Actor);

	
}

void UStudentPerceptorDeckxAlexander::AddZombieToMemory(AActor* Actor)
{
	if (m_SeenZombies.Contains(Actor)) return;
	
	m_SeenZombies.Add(Actor);
	
	
}

void UStudentPerceptorDeckxAlexander::AddPurgeZoneToMemory(AActor* Actor)
{
	if (m_PurgeZones.Contains(Actor)) return;
	m_PurgeZones.Add(Actor);
}


