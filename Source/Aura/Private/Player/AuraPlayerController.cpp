// Copyright Taylor Owen-Milner


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"

#include "Interaction/IHightlightable.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "Components/SplineComponent.h"

#include "Input/AuraInputComponent.h"


AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
	MovementSpline = CreateDefaultSubobject<USplineComponent>("MovementSpline");
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
	AutoRun();
}

void AAuraPlayerController::AutoRun()
{
	if (not bAutoRunning)
		return;
	
	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector LocationOnSpline = MovementSpline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector SplineDirection = MovementSpline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(SplineDirection);

		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	auto AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed,
											&ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AAuraPlayerController::Move(const FInputActionValue& Value)
{
	const FVector2D InputVector = Value.Get<FVector2D>();
	const FRotator Rotation(0.0f, GetControlRotation().Yaw, 0.0f);

	const FVector Forward = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);
	const FVector Right = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(Forward, InputVector.Y);
		ControlledPawn->AddMovementInput(Right, InputVector.X);
	}
}

void AAuraPlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit)
		return;

	LastCursorHit = CurrentCursorHit;
	CurrentCursorHit = CursorHit.GetActor();
	if (LastCursorHit == CurrentCursorHit)
		return;
	
	if (LastCursorHit)
	{
		LastCursorHit->UnHighlight();
	}
	if (CurrentCursorHit)
	{
		CurrentCursorHit->Highlight();
	}
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().Input_LMB))
	{
		bTargeting = CurrentCursorHit != nullptr;
		bAutoRunning = false;
	}
	
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (not GetAsc())
		return;
	
	if (not InputTag.MatchesTagExact(FAuraGameplayTags::Get().Input_LMB) || bTargeting)
	{
		GetAsc()->AbilityInputTagReleased(InputTag);
	}
	else if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().Input_LMB))
	{
		// auto run
		APawn* ControlledPawn = GetPawn();
		if (FollowTime <= ShortPressThreshold && ControlledPawn)
		{
			auto NavigationPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination);
			if (NavigationPath)
			{
				bAutoRunning = true;
				MovementSpline->ClearSplinePoints();
				for (const auto& PathPoint : NavigationPath->PathPoints)
				{
					MovementSpline->AddSplinePoint(PathPoint, ESplineCoordinateSpace::World);
				}
				CachedDestination = NavigationPath->PathPoints.Last();
				MovementSpline->UpdateSpline();
			}
		}
		FollowTime = 0.f; // incremented in AbilityInputTagHeld
	}
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (not GetAsc())
		return;

	if (not InputTag.MatchesTagExact(FAuraGameplayTags::Get().Input_LMB) || bTargeting)
	{
		GetAsc()->AbilityInputTagHeld(InputTag);
	}
	else if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().Input_LMB))
	{
		// manual run
		FollowTime += GetWorld()->GetDeltaSeconds();

		if (CursorHit.bBlockingHit)
		{
			CachedDestination = CursorHit.ImpactPoint;
		}

		if (APawn* ControlledPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}
	
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetAsc()
{
	if (!AuraAbilitySystemComponent)
	{
		auto Asc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>());
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(Asc);
	}

	return AuraAbilitySystemComponent;
}


