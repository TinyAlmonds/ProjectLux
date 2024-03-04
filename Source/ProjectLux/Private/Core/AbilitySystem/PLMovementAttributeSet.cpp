// Copyright TinyAlmonds (Alex Noerdemann)

#include "Core/AbilitySystem/PLMovementAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Misc/Optional.h"

UPLMovementAttributeSet::UPLMovementAttributeSet() : MaxWalkSpeed{600.0f},
																	 JumpZVelocity{1000.0f},
																	 MaxFallSpeed{-1200.0f},
																	 VelocityMultiplierDash{3.0f},
																	 VelocityXYMultiplierWallJump{1.8f},
																	 VelocityZMultiplierWallJump{1.8f},
																	 GravityScaleMultiplierGlide{0.1f},
																	 AirControlGlide{0.5f}
{
}

void UPLMovementAttributeSet::PreAttributeChange(FGameplayAttribute const &Attribute, float &OutNewValue)
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

void UPLMovementAttributeSet::PostGameplayEffectExecute(FGameplayEffectModCallbackData const &Data)
{
	Super::PostGameplayEffectExecute(Data);
}

TOptional<float> UPLMovementAttributeSet::ClampAttributeValue(FGameplayAttribute const &Attribute, float Value)
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
	else if (Attribute == GetMaxFallSpeedAttribute())
	{
		return TOptional<float>{FMath::Min(0.0f, Value)};
	}
	else if (Attribute == GetVelocityXYMultiplierWallJumpAttribute())
	{
		return TOptional<float>{FMath::Max(0.0f, Value)};
	}
	else if (Attribute == GetVelocityZMultiplierWallJumpAttribute())
	{
		return TOptional<float>{FMath::Max(0.0f, Value)};
	}
	else if (Attribute == GetAirControlGlideAttribute())
	{
		return TOptional<float>{FMath::Clamp(Value, 0.0f, 1.0f)};
	}
	else
	{
		return TOptional<float>{};
	}
}