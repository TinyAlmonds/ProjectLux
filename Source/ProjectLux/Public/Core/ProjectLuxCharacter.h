#pragma once


#include "AbilitySystemInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ProjectLuxCharacter.generated.h"

// Forward declarations
class UAbilitySystemComponent;
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

UCLASS()
class PROJECTLUX_API AProjectLuxCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	/** Velocity in z-direction, when the Character is sliding down a wall (range: <0.0 [uu/s]). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Movement", meta = (ClampMax = "0.0"))
	float VelocityZWallSlide;

	/** Scales the Velocity (MaxWalkSpeed of the CharacterMovementComponent) in x/y-direction when the Character performs a WallJump (range: >0.0 [uu/s]). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Movement", meta = (ClampMin = "0.0"))
	float VelocityXYMultiplierWallJump;

	/** Scales the Velocity (JumpZVelocity of the CharacterMovementComponent) in z-direction when the Character performs a WallJump (range: >0.0 [uu/s]). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Movement", meta = (ClampMin = "0.0"))
	float VelocityZMultiplierWallJump;

	/** Sets default values for this character's properties */
	AProjectLuxCharacter();

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
	 * Sets the USplineComponent of the Character to the given value which is needed for the EMovementSpaceState::MovementOnSpline state.
	 * @param MovementSplineComponent - The USplineComponent to set.
	 */
	UFUNCTION(BlueprintCallable, Category = "Character|Movement")
	virtual void SetMovementSpline(USplineComponent const* MovementSplineComponent);

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

	/** Lets the Character sliding down a wall, if the wall sliding flag is True. */
	virtual void OnWallSlidingFlagChanged();

	/** Reduces/extends the space in which the Character can move.*/
	virtual void OnMovementSpaceStateChanged();

	/** Launches the Character of the wall and rotates her towards launch direction. */
	virtual void WallJump();

private:
	/** The AbilitySystemComponent of this Actor. */
	UPROPERTY()
	UAbilitySystemComponent* AbilitySystemComponent;

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

	/**
	 * Checks, whether the Character touches a wall for the wall slide.
	 * @return An TOptional with the FHitResult of the wall when the Character faces and touches the wall, and is currently falling. Otherwise an empty TOptional.
	 */
	TOptional<FHitResult> IsTouchingWallForWallSlide() const;

	/** Updates the rotation of the Character to the last MoveUp-/Right input. This method is called on every Tick. */
	void UpdateRotationToMoveInput();
};
