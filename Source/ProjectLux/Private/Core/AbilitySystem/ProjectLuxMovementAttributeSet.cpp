// Copyright TinyAlmonds (Alex Nördemann)


#include "Core/AbilitySystem/ProjectLuxMovementAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Misc/Optional.h"

UProjectLuxMovementAttributeSet::UProjectLuxMovementAttributeSet()	:
	MaxWalkSpeed{600.0f},
	JumpZVelocity{1000.0f},
	VelocityMultiplierDash{3.0f},
	VelocityXYMultiplierWallJump{1.8f},
	VelocityZMultiplierWallJump{1.8f}
{}

void UProjectLuxMovementAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& OutNewValue)
{
	Super::PreAttributeChange(Attribute, OutNewValue);

	// clamp value
	{
		TOptional<float> ClampedValue = ClampAttributeValue(Attribute, OutNewValue);
		if (ClampedValue)
		{
			OutNewValue = ClampedValue.GetValue();
		}
	}
}

void UProjectLuxMovementAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}

TOptional<float> UProjectLuxMovementAttributeSet::ClampAttributeValue(const FGameplayAttribute& Attribute, const float& Value)
{
	if (Attribute == GetMaxWalkSpeedAttribute())
	{
		return TOptional<float>{FMath::Max(0.0f, Value)};
	}
	else if (Attribute == GetJumpZVelocityAttribute())
	{
		return TOptional<float>{FMath::Max(0.0f, Value)};
	}
	else if (Attribute == GetVelocityMultiplierDashAttribute())
	{
		return TOptional<float>{FMath::Max(0.0f, Value)};
	}
	else if (Attribute == GetVelocityXYMultiplierWallJumpAttribute())
	{
		return TOptional<float>{FMath::Max(0.0f, Value)};
	}
	else if (Attribute == GetVelocityZMultiplierWallJumpAttribute())
	{
		return TOptional<float>{FMath::Max(0.0f, Value)};
	}
	else
	{
		return TOptional<float>{};
	}
}