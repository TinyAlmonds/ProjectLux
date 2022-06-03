// Copyright TinyAlmonds (Alex Nördemann)

#pragma once

#include "AbilitySystemComponent.h"
#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "ProjectLuxMovementAttributeSet.generated.h"

// Uses macros from AttributeSet.h for accessing and initializing attributes
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

// Forward declarations
template<typename OptionalType>
struct TOptional;

/**
 * Class holding all movement related attributes needed for the AProjectLuxCharacter.
 */
UCLASS()
class PROJECTLUX_API UProjectLuxMovementAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UProjectLuxMovementAttributeSet();
	virtual ~UProjectLuxMovementAttributeSet() = default;

	/**
	 * Called just before any modification happens to an attribute. There is no additional context provided here since anything can trigger this: Executed effects, duration based effects, effects being removed, immunity being applied, stacking rules changing, etc.
	 * @param Attribute - The attribute whose value will be changed.
	 * @param OutNewValue - The attribute's new value. This is a mutable reference so you are able to clamp the newly applied value as well.
	 */
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& OutNewValue) override;

	/**
	 * Called just before a GameplayEffect is executed to modify the base value of an attribute. No more changes can be made. Note: This is only called during an 'execute'. E.g., a modification to the 'base value' of an attribute. It is not called during an application of a GameplayEffect, such as a 5 ssecond +10 movement speed buff.
	 * @param Data - Data struct holding necessary information of the executed/applied GameplayEffect.
	*/
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	/** Maximum walk speed of the character [uu/s].*/
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData MaxWalkSpeed;
	ATTRIBUTE_ACCESSORS(UProjectLuxMovementAttributeSet, MaxWalkSpeed)

	/** Jump velocity in z-direction of the character [uu/s].*/
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData JumpZVelocity;
	ATTRIBUTE_ACCESSORS(UProjectLuxMovementAttributeSet, JumpZVelocity)

	/** Maximum fall speed (velocity in z-direction) of the character [uu/s].*/
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData MaxFallSpeed;
	ATTRIBUTE_ACCESSORS(UProjectLuxMovementAttributeSet, MaxFallSpeed)

	/** Velocity  multiplier of the Dash ability of the character.*/
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData VelocityMultiplierDash;
	ATTRIBUTE_ACCESSORS(UProjectLuxMovementAttributeSet, VelocityMultiplierDash)

	/** Velocity  multiplier in xy-direction of the Wall Jump ability of the character.*/
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData VelocityXYMultiplierWallJump;
	ATTRIBUTE_ACCESSORS(UProjectLuxMovementAttributeSet, VelocityXYMultiplierWallJump)

	/** Velocity  multiplier in z-direction of the Wall Jump ability of the character.*/
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData VelocityZMultiplierWallJump;
	ATTRIBUTE_ACCESSORS(UProjectLuxMovementAttributeSet, VelocityZMultiplierWallJump)

	/** Gravity scale multiplier of the Glide ability of the character.*/
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData GravityScaleMultiplierGlide;
	ATTRIBUTE_ACCESSORS(UProjectLuxMovementAttributeSet, GravityScaleMultiplierGlide)

	/** AirControl of the Glide ability of the character.*/
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData AirControlGlide;
	ATTRIBUTE_ACCESSORS(UProjectLuxMovementAttributeSet, AirControlGlide)

private:
	/**
	 * Clamps the passed value of the passed attribute.
	 * @param Attribute - The attribute whose value should be clamped.
	 * @param Value - The attribute's value to clamp.
	 * @return An Optional with the clamped value, if the attribute is known to this AttributeSet; else an empty Optional.
	 */
	TOptional<float> ClampAttributeValue(const FGameplayAttribute& Attribute, const float& Value);
};
