// Copyright Taylor Owen-Milner


#include "Actor/ComponentPoolContainer.h"

AComponentPoolContainer::AComponentPoolContainer()
{
	PrimaryActorTick.bCanEverTick = false;
	ComponentPool.Init(nullptr, ChunkSize);
}

UPrimitiveComponent* AComponentPoolContainer::GetPooledComponent()
{
	UPrimitiveComponent* Component = nullptr;
	int InitialIndex = 0;
	while (not Component)
	{
		for (int i = InitialIndex; i < ComponentPool.Num(); i++)
		{
			if (not Component)
			{
				Component = NewObject<UPrimitiveComponent>(this, ComponentClass);
				ComponentPool[i] = Component;
				break;
			}
			
			if (not Component->IsActive())
			{
				Component = ComponentPool[i];
				break;
			}
		}

		if (not Component)
		{
			for (int i = 0; i < ChunkSize; i++)
			{
				ComponentPool.Add(nullptr);
			}
			InitialIndex += ChunkSize;
		}
	}

	Component->RegisterComponent();
	return Component;
}

void AComponentPoolContainer::ReturnToPool(UPrimitiveComponent* Component)
{
	Component->UnregisterComponent();
}

void AComponentPoolContainer::BeginPlay()
{
	Super::BeginPlay();

	if (bPreAllocateFirstChunk)
	{
		for (int i = 0; i < ChunkSize; i++)
		{
			ComponentPool[i] = NewObject<UPrimitiveComponent>(this, ComponentClass);
		}
	}
}

