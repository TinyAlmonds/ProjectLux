// Copyright TinyAlmonds (Alex Noerdemann)

#include "Core/PLEnemyCharacterBase.h"

#include "Abilities/GameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"

#include "Core/AbilitySystem/PLAbilitySystemComponent.h"
#include "Core/AbilitySystem/PLCharacterAttributeSet.h"

APLEnemyCharacterBase::APLEnemyCharacterBase() : DeadTag{FGameplayTag::RequestGameplayTag(FName("Status.Dead"))}
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Construct the ASC
	AbilitySystemComponent = CreateDefaultSubobject<UPLAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	// Construct the attribute sets
	AttributeSet = CreateDefaultSubobject<UPLCharacterAttributeSet>(TEXT("AttributeSet"));
}

void APLEnemyCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UAbilitySystemComponent *APLEnemyCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void APLEnemyCharacterBase::PossessedBy(AController *NewController)
{
	Super::PossessedBy(NewController);

	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	// add default abilities to ASC
	for (TSubclassOf<UGameplayAbility> const &DefaultAbility : DefaultAbilities)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(DefaultAbility, 1, -1, this));
	}

	// initialize AttributeSet by an instant GameplayEffect (which does exactly this)
	if (IsValid(AttributeSetInitEffect))
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);
		AbilitySystemComponent->ApplyGameplayEffectToSelf(AttributeSetInitEffect.GetDefaultObject(), 1.0f, EffectContext);
	}

	// add delegates to attribute changes
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &APLEnemyCharacterBase::OnHealthChanged);

	// add delegates to GameplayTag changes
	AbilitySystemComponent->RegisterGameplayTagEvent(DeadTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &APLEnemyCharacterBase::OnDeadTagChanged);
}

void APLEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void APLEnemyCharacterBase::OnHealthChanged(FOnAttributeChangeData const &Data)
{
	HealthChanged(Data.OldValue, Data.NewValue);
}

void APLEnemyCharacterBase::HealthChanged_Implementation(float OldValue, float NewValue)
{
	if (NewValue <= 0.0f)
	{
		AbilitySystemComponent->AddLooseGameplayTag(DeadTag);
	}
}

void APLEnemyCharacterBase::OnDeadTagChanged(const FGameplayTag, int32 NewCount)
{
	DeadTagChanged(NewCount);
}

void APLEnemyCharacterBase::DeadTagChanged_Implementation(int32 NewCount)
{
	// signal character death on dead tag application
	if (NewCount == 1)
	{
		// TODO: add default handling of enemy character death
	}
}
