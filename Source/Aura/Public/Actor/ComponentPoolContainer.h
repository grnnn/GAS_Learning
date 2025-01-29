// Copyright Taylor Owen-Milner

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ComponentPoolContainer.generated.h"

UCLASS()
class AURA_API AComponentPoolContainer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AComponentPoolContainer();

	UPrimitiveComponent* GetPooledComponent();
	void ReturnToPool(UPrimitiveComponent* Component);
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPrimitiveComponent> ComponentClass;

	UPROPERTY(EditDefaultsOnly)
	int ChunkSize = 20;

	UPROPERTY(EditDefaultsOnly)
	bool bPreAllocateFirstChunk = false;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TArray<UPrimitiveComponent*> ComponentPool;

};
