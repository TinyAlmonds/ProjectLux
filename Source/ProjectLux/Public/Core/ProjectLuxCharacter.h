#pragma once


#include "AbilitySystemInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "ProjectLuxCharacter.generated.h"


// Forward declarations
class UGameplayAbility;
class UGameplayEffect;
class UAbilitySystemComponent;
class UProjectLuxCharacterAttributeSet;
class UProjectLuxMovementAttributeSet;
struct FOnAttributeChangeData;
template<typename OptionalType>
struct TOptional;
struct FHitResult;
class USplineComponent;

/** Enum indicating the space in which the Character is able to move. */
UENUM(BlueprintType)
enum class EMovementSpaceState : uint8
{
	MovementIn2D,
	MovementIn3D,
	MovementOnSpline
};

/** Struct holding default values for the ProjectLuxCharacter. */
USTRUCT()
struct FProjectLuxCharacterDefaultValues
{
	GENERATED_BODY()

	FProjectLuxCharacterDefaultValues() {};

	float VelocityZWallSlide{ -180.0f };
	float CharacterJumpMaxHoldTime{ 0.35f };
	float CharacterMovementComponentGravityScale{ 5.5f };
	float CharacterMovementComponentMaxAcceleration{ 8192.0f };
	float CharacterMovementComponentGroundFriction{ 8.0f };
	float CharacterMovementComponentBrakingDecelerationFalling{ 2048.0f };
	float CharacterMovementComponentAirControl{ 1.0f };
	float CharacterMovementComponentAirControlBoostMultiplier{ 0.0f };
	float CharacterMovementComponentAirControlBoostVelocityThreshold{ 0.0f };
	FRotator CharacterMovementComponentRotationRate = FRotator(0.0f, 30.0f, 0.0f);
};


UCLASS()
class PROJECTLUX_API AProjectLuxCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	/** Maximum falling velocity of the Character (three times the value of the MaxWalkSpeed) (range: <0.0 [uu/s]). */
	static constexpr float MaxFallVelocity{ -3.0f * 600.0f };

	/** Velocity in z-direction, when the Character is sliding down a wall (range: <0.0 [uu/s]). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Movement", meta = (ClampMax = "0.0"))
	float VelocityZWallSlide;
	
	/** Default GameplayAbilities for this character. These will be removed and added again on character possession. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

	/** GameplayEffect which is used to initialize the AttributeSet of the character. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Abilities")
	TSubclassOf<UGameplayEffect> AttributeSetInitEffect;

	/** GameplayEffect which is used to initialize the movement related AttributeSet of the character. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Abilities")
	TSubclassOf<UGameplayEffect> MovementAttributeSetInitEffect;

	/** Sets default values for this character's properties */
	AProjectLuxCharacter();

	/** Called after the C++ constructor and after the properties have been initialized, including those loaded from config. */
	virtual void PostInitProperties() override;

	/** Called every frame */
	virtual void Tick(float DeltaTime) override;

	/**
	 * Returns the AbilitySystemComponent (ASC) of this Actor.
	 * @return The AbilitySystemComponent (ASC) of this Actor.
	 */
	UFUNCTION(BlueprintCallable, Category = "Character|Abilities")
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	/** Runs logic when this Character is possessed. */
	virtual void PossessedBy(AController* NewController) override;

	/** Performs a wall jump when the Character is wall sliding otherwise a jump until the jump button is released or exceeds the max hold time. */
	UFUNCTION(BlueprintCallable, Category = "Character|Movement")
	virtual void JumpPress();

	/** Stops the jump of the Character. */
	UFUNCTION(BlueprintCallable, Category = "Character|Movement")
	virtual void JumpRelease();

	/**
	 * Moves the Character to the right and updates the related axis value member.
	 * @param AxisValue - The axis value to set (range: -1.0 to 1.0).
	 */
	UFUNCTION(BlueprintCallable, Category = "Character|Movement")
	virtual void MoveRight(float AxisValue);

	/**
	 * Moves the Character up and updates the related axis value member.
	 * @param AxisValue - The axis value to set (range: -1.0 to 1.0).
	 */
	UFUNCTION(BlueprintCallable, Category = "Character|Movement")
	virtual void MoveUp(float AxisValue);

	/** Performs the dash ability when the Character has this ability. */
	UFUNCTION(BlueprintCallable, Category = "Character|Movement")
	virtual void DashPress();

	/** Performs the attack ability when the Character has this ability. */
	UFUNCTION(BlueprintCallable, Category = "Character|Combat")
	virtual void AttackPress();

	/**
	 * Returns the current value of the wall sliding flag.
	 * @return True if the Character is wall sliding, False otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "Character|Movement")
	virtual bool GetWallSlidingFlag() const;

	/**
	 * Returns the current value of the movement space state.
	 * @return The current value of the movement space state member.
	 */
	UFUNCTION(BlueprintCallable, Category = "Character|Movement")
	virtual EMovementSpaceState GetMovementSpaceState() const;

	/**
	 * Sets the movement space state of the Character to the given value and triggers a logic on state changes.
	 * @param State - The EMovementSpaceState to set.
	 */
	UFUNCTION(BlueprintCallable, Category = "Character|Movement")
	virtual void SetMovementSpaceState(EMovementSpaceState State);

	/**
	 * Returns the previous value of the movement space state.
	 * @return The previous value of the movement space state member.
	 */
	UFUNCTION(BlueprintCallable, Category = "Character|Movement")
	virtual EMovementSpaceState GetPreviousMovementSpaceState() const;

	/**
	 * Sets the USplineComponent of the Character to the given value which is needed for the EMovementSpaceState::MovementOnSpline state.
	 * @param MovementSplineComponent - The USplineComponent to set.
	 */
	UFUNCTION(BlueprintCallable, Category = "Character|Movement")
	virtual void SetMovementSpline(USplineComponent const* MovementSplineComponent);

	/**
	 * Activates the combo for the attack ability of the Character.
	 * @param ComboNextSectionName - The name of the next section in the AnimMontage of the attack ability.
	 */
	UFUNCTION(BlueprintCallable, Category = "Character|Combat")
	virtual void ActivateAttackAbilityCombo(FName ComboNextSectionName);

	/** Deactivates the combo for the attack ability of the Character.  */
	UFUNCTION(BlueprintCallable, Category = "Character|Combat")
	virtual void DeactivateAttackAbilityCombo();

	/** Checks if the Character is dead.
	 * @return True if the Character is dead; False otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "Character")
	virtual bool IsDead();

protected:
	/** Called when the game starts or when spawned. */
	virtual void BeginPlay() override;

	/** Determines if the Character should wall slide and sets the related flag. This method is called on every Tick. */
	virtual void UpdateWallSlidingFlag();

	/**
	 * Sets the wall sliding flag of the Character to the given value and triggers a behavior to the according flag value.
	 * @param bFlagValue - The flag value to set.
	 */
	virtual void SetWallSlidingFlag(bool bFlagValue);

	/** Called, when SetWallSlidingFlag() was called. Tries to activate the WallSlide ability if wall slide flag is True; else the ability is canceled.*/
	virtual void OnWallSlidingFlagSet();

	/** Reduces/extends the space in which the Character can move.*/
	virtual void OnMovementSpaceStateChanged();

	/** Event for the Blueprint class to react on MovementSpaceState changes.*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Character|Movement", DisplayName = "On MovementSpaceState Changed")
	void MovementSpaceStateChanged();

	/** Launches the Character of the wall and rotates her towards launch direction. */
	UFUNCTION(BlueprintCallable, Category = "Character|Movement|Ability")
	virtual void WallJump();

	/** Dashes the Character rotates her towards dash direction. */
	UFUNCTION(BlueprintCallable, Category = "Character|Movement|Ability")
	virtual void Dash();

	/** Stops the dash of the Character. */
	UFUNCTION(BlueprintCallable, Category = "Character|Movement|Ability")
	virtual void StopDash();

	/** Reacts to Health attribute changes and calls the Blueprint event.*/
	void OnHealthChanged(const FOnAttributeChangeData& Data);

	/** Event for the Blueprint class to react to Health changes of the AttributeSet.*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Character|Attributes", DisplayName = "On Health Changed")
	void HealthChanged(float OldValue, float NewValue);

	/** Reacts to MaxWalkSpeed attribute changes.*/
	void OnMaxWalkSpeedAttributeChanged(const FOnAttributeChangeData& Data);

	/** Reacts to JumpZVelocity attribute changes.*/
	void OnJumpZVelocityAttributeChanged(const FOnAttributeChangeData& Data);

	/**
	 * Reacts to changes of the ASC, when the "Status.Dead" tag is applied or removed.
	 * @param Unused. Only for interface call.
	 * @param NewCount - The new count of this tag.
	 */
	virtual void DeadTagChanged(const FGameplayTag, int32 NewCount);

	/** Event for the Blueprint class to react to character death.*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Character", DisplayName = "On Died")
	void Died();

private:
	/** The default values for various members of this Character. */
	FProjectLuxCharacterDefaultValues DefaultValues;

	/** The AbilitySystemComponent of this Actor. */
	UPROPERTY()
	UAbilitySystemComponent* AbilitySystemComponent;

	/** List of attributes modified by the ability system */
	UPROPERTY()
	UProjectLuxCharacterAttributeSet* AttributeSet;

	/** List of movement related attributes modified by the ability system */
	UPROPERTY()
	UProjectLuxMovementAttributeSet* MovementAttributeSet;

	/** Member holding the last set value of the MoveUp axis mapping. */
	float AxisValueMoveUp;

	/** Member holding the last set value of the MoveRight axis mapping. */
	float AxisValueMoveRight;

	/** Member indicating whether the Character should wall slide or not. */
	bool bWallSlidingFlag;

	/** Member indicating the space the Character is currently able to move in. */
	EMovementSpaceState MovementSpace;

	/** Member indicating the space the Character was able to move in before the change. */
	EMovementSpaceState PreviousMovementSpace;

	/** Reference to an USplineComponent in the world on which the Character moves, if she is in the EMovementSpaceState::MovementOnSpline state. */
	UPROPERTY()
	USplineComponent const* MovementSplineComponentFromWorld;

	/** Member holding the tag which describes the Wall Slide ability. */
	FGameplayTag WallSlideAbilityTag;

	/** Member holding the tag which describes the Wall Jump ability. */
	FGameplayTag WallJumpAbilityTag;

	/** Member holding the tag which describes the Dash ability. */
	FGameplayTag DashAbilityTag;

	/** Member holding the tag which describes the Double-Dash ability. */
	FGameplayTag DoubleDashAbilityTag;

	/** Member holding the tag which describes the Attack ability. */
	FGameplayTag AttackAbilityTag;

	/** Member holding the tags of abilities blocking the MoveRight-/Up input. */
	FGameplayTagContainer MoveBlockingAbilityTags;

	/** Member holding the tag which describes the death of the character. */
	FGameplayTag DeadTag;

	/** Flag indicating whether the AnimMontage of the attack ability is in a combo interval/window. If so the flag is True; otherwise False.*/
	bool bAttackAbilityComboEnabled;

	/** The name of the next section in the AnimMontage of the attack ability. */
	FName AttackAbilityNextSectionCombo;

	/** FHitResult of the last valid IsTouchingWallForWallSlide() method call. Only use it when the Character is wall sliding.*/
	FHitResult LastValidWallSlideHitResult;

	/**
	 * Checks, whether the Character touches a wall for the wall slide.
	 * @return An TOptional with the FHitResult of the wall when the Character faces and touches the wall, and is currently falling. Otherwise an empty TOptional.
	 */
	TOptional<FHitResult> IsTouchingWallForWallSlide();

	/** Updates the rotation of the Character to the last MoveUp-/Right input. This method is called on every Tick. */
	void UpdateRotationToMoveInput();
};
