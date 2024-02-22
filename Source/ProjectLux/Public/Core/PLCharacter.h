// Copyright TinyAlmonds (Alex Noerdemann)
#pragma once

#include "AbilitySystemInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"

#include "Types/PLMovementSpaceState.h"
#include "PLCharacter.generated.h"

// Forward declarations
class UGameplayAbility;
class UGameplayEffect;
class UAbilitySystemComponent;
class UPLCharacterAttributeSet;
class UPLMovementAttributeSet;
struct FOnAttributeChangeData;
template <typename OptionalType>
struct TOptional;
struct FHitResult;
class USplineComponent;

/**
 * Class for the main playable character of the game.
 */
UCLASS()
class PROJECTLUX_API APLCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	/** Default GameplayAbilities for this character. These will be removed and added again on character possession. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

	/** Default passive GameplayAbilities for this character. These will be removed and added again on character possession. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> DefaultPassiveAbilities;

	/** GameplayEffect which is used to initialize the AttributeSet of the character. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Abilities")
	TSubclassOf<UGameplayEffect> AttributeSetInitEffect;

	/** GameplayEffect which is used to initialize the movement related AttributeSet of the character. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Abilities")
	TSubclassOf<UGameplayEffect> MovementAttributeSetInitEffect;

	/** Sets default values for this character's properties */
	APLCharacter();

	/** Called every frame */
	virtual void Tick(float DeltaTime) override;

	/**
	 * Returns the AbilitySystemComponent (ASC) of this Actor.
	 * @return The AbilitySystemComponent (ASC) of this Actor.
	 */
	UFUNCTION(BlueprintCallable, Category = "Character|Abilities")
	virtual UAbilitySystemComponent *GetAbilitySystemComponent() const override;

	/** Runs logic when this Character is possessed. */
	virtual void PossessedBy(AController *NewController) override;

	/** Performs a wall jump when the Character is wall sliding otherwise a jump until the jump button is released or exceeds the max hold time. */
	UFUNCTION(BlueprintCallable, Category = "Character|Movement")
	virtual void JumpPress();

	/** Stops the jump of the Character. */
	UFUNCTION(BlueprintCallable, Category = "Character|Movement")
	virtual void JumpRelease();

	/**
	 * Updates the right axis value member to the given input.
	 * @param AxisValue - The axis value to set (range: -1.0 to 1.0).
	 */
	UFUNCTION(BlueprintCallable, Category = "Character|Movement")
	virtual void MoveRight(float AxisValue);

	/**
	 * Updates the up axis value member to the given input.
	 * @param AxisValue - The axis value to set (range: -1.0 to 1.0).
	 */
	UFUNCTION(BlueprintCallable, Category = "Character|Movement")
	virtual void MoveUp(float AxisValue);

	/** Activates the Sprint Ability of the character if conditions are met. */
	UFUNCTION(BlueprintCallable, Category = "Character|Movement")
	virtual void SprintPress();

	/** Cancels the Sprint Ability of the character. */
	UFUNCTION(BlueprintCallable, Category = "Character|Movement")
	virtual void SprintRelease();

	/** Performs the dash ability when the Character has this ability. */
	UFUNCTION(BlueprintCallable, Category = "Character|Movement")
	virtual void DashPress();

	/** Activates the QuickStep Ability of the character if conditions are met. */
	UFUNCTION(BlueprintCallable, Category = "Character|Movement")
	virtual void QuickStepPress();

	/** Performs the Glide ability when the Character has this ability. */
	UFUNCTION(BlueprintCallable, Category = "Character|Movement")
	virtual void GlidePress();

	/** Cancels/stops the Glide ability when the Character has this ability and it's active. */
	UFUNCTION(BlueprintCallable, Category = "Character|Movement")
	virtual bool TryCancelGlideAbility();

	/** Performs the attack ability when the Character has this ability. */
	UFUNCTION(BlueprintCallable, Category = "Character|Combat")
	virtual void AttackPress();

	/**
	 * Returns the current value of the wall sliding flag.
	 * @return True if the Character is wall sliding, False otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "Character|Movement")
	bool GetWallSlidingFlag() const;

	/**
	 * Returns the current value of the movement space state.
	 * @return The current value of the movement space state member.
	 */
	UFUNCTION(BlueprintCallable, Category = "Character|Movement")
	EPLMovementSpaceState GetMovementSpaceState() const;

	/**
	 * Sets the movement space state of the Character to the given value and triggers a logic on state changes.
	 * @param State - The EPLMovementSpaceState to set.
	 */
	UFUNCTION(BlueprintCallable, Category = "Character|Movement")
	virtual void SetMovementSpaceState(EPLMovementSpaceState State);

	/**
	 * Returns the previous value of the movement space state.
	 * @return The previous value of the movement space state member.
	 */
	UFUNCTION(BlueprintCallable, Category = "Character|Movement")
	EPLMovementSpaceState GetPreviousMovementSpaceState() const;

	/**
	 * Returns the USplineComponent in the world on which the Character player can move, if in the EPLMovementSpaceState::MovementOnSpline state.
	 * @return The spline component member. Has to be checked for validness.
	 */
	UFUNCTION(BlueprintCallable, Category = "Character|Movement")
	const USplineComponent *GetMovementSplineComponent();

	/**
	 * Sets the USplineComponent of the Character to the given value which is needed for the EPLMovementSpaceState::MovementOnSpline state.
	 * @param MovementSplineComponent - The USplineComponent to set.
	 */
	UFUNCTION(BlueprintCallable, Category = "Character|Movement")
	virtual void SetMovementSpline(USplineComponent const *MovementSplineComponent);

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

	/** Reacts to Health attribute changes and calls the Blueprint event.*/
	void OnHealthChanged(FOnAttributeChangeData const &Data);

	/** Event for the Blueprint class to react to Health changes of the AttributeSet.*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Character|Attributes", DisplayName = "On Health Changed")
	void HealthChanged(float OldValue, float NewValue);

	/** Reacts to MaxWalkSpeed attribute changes.*/
	void OnMaxWalkSpeedAttributeChanged(FOnAttributeChangeData const &Data);

	/** Reacts to JumpZVelocity attribute changes.*/
	void OnJumpZVelocityAttributeChanged(FOnAttributeChangeData const &Data);

	/**
	 * Reacts to changes of the ASC, when the "Status.Dead" tag is applied or removed.
	 * @param Unused. Only for interface call.
	 * @param NewCount - The new count of this tag.
	 */
	virtual void DeadTagChanged(const FGameplayTag, int32 NewCount);

	/** Event for the Blueprint class to react to character death.*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Character", DisplayName = "On Died")
	void Died();

	/**
	 * Checks, whether the Character touches a wall for the wall slide.
	 * @return An TOptional with the FHitResult of the wall when the Character faces and touches the wall, and is currently falling. Otherwise an empty TOptional.
	 */
	virtual TOptional<FHitResult> IsTouchingWallForWallSlide();

	/**
	 * Returns the movement direction depending on the EPLMovementSpaceState of the Character to the given input vector. This method is called on every Tick.
	 * @return The extracted movement direction.
	 */
	virtual FVector GetMoveDirectionFromMoveInput(FVector2D MoveInputVector) const;

	/**
	 * Updates the rotation of the Character to the given movement direction. This method is called on every Tick.
	 * @param MoveDirection - The extracted movement direction.
	 */
	virtual void UpdateRotationToMoveDirection(FVector MovementDirection);

	/**
	 * Tries to rotate the character away from the wall which causes the WallSlide ability. If the input hits a certain threshold away from the wall, the rotation will succeed.
	 * @param RotationFromInput - The input to which the character should rotate.
	 */
	virtual void TryRotateAwayFromWall(FRotator3d const &RotationFromInput);

	/**
	 * Member holds the default value of the CharacterMovementComponent's GravityScale
	 * @note This is a little flaw in the class design, since this value has to kept in sync with the constant default value in the related Blueprint class.
	 */
	static constexpr float DefaultCharacterMovementComponentGravityScale{5.5f};

	/** The AbilitySystemComponent of this Actor. */
	UPROPERTY()
	UAbilitySystemComponent *AbilitySystemComponent;

	/** List of attributes modified by the ability system */
	UPROPERTY()
	UPLCharacterAttributeSet *AttributeSet;

	/** List of movement related attributes modified by the ability system */
	UPROPERTY()
	UPLMovementAttributeSet *MovementAttributeSet;

	/** Member holding the last set value of the MoveUp axis mapping. */
	UPROPERTY(BlueprintReadOnly, Category = "Character|Movement")
	float AxisValueMoveUp{};

	/** Member holding the last set value of the MoveRight axis mapping. */
	UPROPERTY(BlueprintReadOnly, Category = "Character|Movement")
	float AxisValueMoveRight{};

	/** Member holding the last determined value of the movement direction derived from the user input. */
	UPROPERTY(BlueprintReadOnly, Category = "Character|Movement")
	FVector MoveDirection{};

	/** Member indicating whether the Character should wall slide or not. */
	bool bWallSlidingFlag;

	/** Member indicating the space the Character is currently able to move in. */
	EPLMovementSpaceState MovementSpace;

	/** Member indicating the space the Character was able to move in before the change. */
	EPLMovementSpaceState PreviousMovementSpace;

	/** Reference to an USplineComponent in the world on which the Character moves, if she is in the EPLMovementSpaceState::MovementOnSpline state. */
	UPROPERTY()
	USplineComponent const *MovementSplineComponentFromWorld;

	/** Member holding the tag which describes the Sprint ability. */
	FGameplayTag SprintAbilityTag;

	/** Member holding the tag which describes the Wall Slide ability. */
	FGameplayTag WallSlideAbilityTag;

	/** Member holding the tag which describes the Wall Jump ability. */
	FGameplayTag WallJumpAbilityTag;

	/** Member holding the tag which describes the Dash ability. */
	FGameplayTag DashAbilityTag;

	/** Member holding the tag which describes the Double-Dash ability. */
	FGameplayTag DoubleDashAbilityTag;

	/** Member holding the tag which describes the QuickStep ability. */
	FGameplayTag QuickStepAbilityTag;

	/** Member holding the tag which describes the Glide ability. */
	FGameplayTag GlideAbilityTag;

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
};
