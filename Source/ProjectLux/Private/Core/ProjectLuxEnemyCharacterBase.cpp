// Copyright TinyAlmonds (Alex Nördemann)

#include "Core/ProjectLuxEnemyCharacterBase.h"

#include "Abilities/GameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"

#include "Core/AbilitySystem/ProjectLuxAbilitySystemComponent.h"
#include "Core/AbilitySystem/ProjectLuxCharacterAttributeSet.h"


AProjectLuxEnemyCharacterBase::AProjectLuxEnemyCharacterBase() :
	DeadTag{ FGameplayTag::RequestGameplayTag(FName("Status.Dead")) }
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Construct the ASC
	AbilitySystemComponent = CreateDefaultSubobject<UProjectLuxAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	// Construct the attribute sets
	AttributeSet = CreateDefaultSubobject<UProjectLuxCharacterAttributeSet>(TEXT("AttributeSet"));
}

void AProjectLuxEnemyCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UAbilitySystemComponent* AProjectLuxEnemyCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AProjectLuxEnemyCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	// add default abilities to ASC
	for (TSubclassOf<UGameplayAbility> const& DefaultAbility : DefaultAbilities)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(DefaultAbility, 1, -1, this));
	}

	// initialize AttributeSet by an instant GameplayEffect (which does exactly this)
	if(IsValid(AttributeSetInitEffect))
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);
		AbilitySystemComponent->ApplyGameplayEffectToSelf(AttributeSetInitEffect.GetDefaultObject(), 1.0f, EffectContext);
	}

	// add delegates to attribute changes
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &AProjectLuxEnemyCharacterBase::OnHealthChanged);

	// add delegates to GameplayTag changes
	AbilitySystemComponent->RegisterGameplayTagEvent(DeadTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AProjectLuxEnemyCharacterBase::OnDeadTagChanged);
}

void AProjectLuxEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();	
}

void AProjectLuxEnemyCharacterBase::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	HealthChanged(Data.OldValue, Data.NewValue);
}

void AProjectLuxEnemyCharacterBase::HealthChanged_Implementation(float OldValue, float NewValue)
{
	if (NewValue <= 0.0f)
	{
		AbilitySystemComponent->AddLooseGameplayTag(DeadTag);
	}
}

void AProjectLuxEnemyCharacterBase::OnDeadTagChanged(const FGameplayTag, int32 NewCount)
{
	DeadTagChanged(NewCount);
}

void AProjectLuxEnemyCharacterBase::DeadTagChanged_Implementation(int32 NewCount)
{
	// signal character death on dead tag application
	if (NewCount == 1)
	{
		//TODO: add default handling of enemy character death
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("Enemy died!")); //DEBUG
	}
}
