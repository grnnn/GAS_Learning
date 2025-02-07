// Copyright Taylor Owen-Milner


#include "Character/AuraEnemy.h"

#include "AuraGameplayTags.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Aura/Aura.h"
#include "Components/WidgetComponent.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/EnemyWidgetController.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	InitAbilityActorInfo();
	CreateEnemyWidgetController();
	RegisterGameplayTagEvents();
}

void AAuraEnemy::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	InitializeDefaultData();
}

void AAuraEnemy::InitializeDefaultData()
{
	UAuraAbilitySystemLibrary::InitializeDefaultAttributes(this, AbilitySystemComponent, Class, Level);
	UAuraAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent);
}

void AAuraEnemy::CreateEnemyWidgetController()
{
	if (EnemyWidgetController)
		return;

	EnemyWidgetController = NewObject<UEnemyWidgetController>(this);
	EnemyWidgetController->SetWidgetControllerParams(FWidgetControllerParams(nullptr, nullptr, AbilitySystemComponent, AttributeSet, nullptr));

	if (auto Widget = Cast<UAuraUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		Widget->SetWidgetController(EnemyWidgetController);
	}
	
	EnemyWidgetController->BroadcastInitialValues();
	EnemyWidgetController->BindCallbacksToDependencies();
}

void AAuraEnemy::RegisterGameplayTagEvents()
{
	AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Effects_HitReact, EGameplayTagEventType::NewOrRemoved)
		.AddLambda([this](const FGameplayTag Tag, int32 Count)
		{
			bHitReacting = Count > 0;
			GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0 : BaseWalkSpeed;
		});
}

void AAuraEnemy::Highlight()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	check(Weapon);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AAuraEnemy::UnHighlight()
{
	GetMesh()->SetRenderCustomDepth(false);
	GetMesh()->SetCustomDepthStencilValue(0);
	check(Weapon);
	Weapon->SetRenderCustomDepth(false);
	Weapon->SetCustomDepthStencilValue(0);
}

int32 AAuraEnemy::GetCombatLevel()
{
	return Level;
}

void AAuraEnemy::Die()
{
	SetLifeSpan(DeathLifespan);
	Super::Die();
}
