// Copyright TinyAlmonds (Alex Noerdemann)

#include "Core/ProjectLuxCharacter.h"

#include "Abilities/GameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SplineComponent.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayEffectTypes.h"
#include "Math/UnrealMathUtility.h"
#include "Misc/Optional.h"

#include "Core/ProjectLuxPlayerController.h"
#include "Core/AbilitySystem/ProjectLuxCharacterAttributeSet.h"
#include "Core/AbilitySystem/ProjectLuxMovementAttributeSet.h"

AProjectLuxCharacter::AProjectLuxCharacter() : AxisValueMoveUp{0.0f},
											   AxisValueMoveRight{0.0f},
											   bWallSlidingFlag{false},
											   MovementSpace{EMovementSpaceState::MovementIn3D},
											   PreviousMovementSpace{EMovementSpaceState::MovementIn3D},
											   MovementSplineComponentFromWorld{nullptr},
											   SprintAbilityTag{FGameplayTag::RequestGameplayTag(FName("Ability.Movement.Sprint"))},
											   WallSlideAbilityTag{FGameplayTag::RequestGameplayTag(FName("Ability.Movement.WallSlide"))},
											   WallJumpAbilityTag{FGameplayTag::RequestGameplayTag(FName("Ability.Movement.WallJump"))},
											   DashAbilityTag{FGameplayTag::RequestGameplayTag(FName("Ability.Movement.Dash"))},
											   DoubleDashAbilityTag{FGameplayTag::RequestGameplayTag(FName("Ability.Movement.DoubleDash"))},
											   QuickStepAbilityTag{FGameplayTag::RequestGameplayTag(FName("Ability.Movement.QuickStep"))},
											   GlideAbilityTag{FGameplayTag::RequestGameplayTag(FName("Ability.Movement.Glide"))},
											   AttackAbilityTag{FGameplayTag::RequestGameplayTag(FName("Ability.Combat.Attack"))},
											   DeadTag{FGameplayTag::RequestGameplayTag(FName("Status.Dead"))}
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Construct the ASC
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	// Construct the attribute sets
	AttributeSet = CreateDefaultSubobject<UProjectLuxCharacterAttributeSet>(TEXT("AttributeSet"));
	MovementAttributeSet = CreateDefaultSubobject<UProjectLuxMovementAttributeSet>(TEXT("MovementAttributeSet"));

	// Fill the FGameplayTagContainer which blocking certain inputs/abilities
	MoveBlockingAbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Reject.MoveInput")));
	MoveBlockingAbilityTags.AddTag(DashAbilityTag);
	MoveBlockingAbilityTags.AddTag(DoubleDashAbilityTag);
	MoveBlockingAbilityTags.AddTag(QuickStepAbilityTag);
}

void AProjectLuxCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UCharacterMovementComponent *CharacterMovementComponent = GetCharacterMovement();

	UpdateWallSlidingFlag();

	// snap Character to closest Spline location, when in Spline Movement
	if ((MovementSpace == EMovementSpaceState::MovementOnSpline) && MovementSplineComponentFromWorld)
	{
		FVector CharacterWorldLocation = GetRootComponent()->GetComponentLocation();
		// we only want to find the closest location on the spline in the XY plane, since the Character can move freely in the Z direction
		FVector ClosestWorldLocationOnSpline = MovementSplineComponentFromWorld->FindLocationClosestToWorldLocation(FVector{CharacterWorldLocation.X, CharacterWorldLocation.Y, 0.0f}, ESplineCoordinateSpace::World);
		GetRootComponent()->SetWorldLocation(FVector{ClosestWorldLocationOnSpline.X, ClosestWorldLocationOnSpline.Y, CharacterWorldLocation.Z});
	}

	if (AbilitySystemComponent)
	{
		// only allow rotation to movement input when "movement blocking ability" are inactive
		if (AbilitySystemComponent->HasAnyMatchingGameplayTags(MoveBlockingAbilityTags) == false)
		{
			UpdateMovementToMoveInput();
			UpdateRotationToMoveInput();
		}
		else
		{
			// stop jumping, when "movement blocking abilities" are active
			if (CharacterMovementComponent)
			{
				if (CharacterMovementComponent->IsFalling())
				{
					StopJumping();
				}
			}

			// rotate Character while moving on a Spline
			if ((MovementSpace == EMovementSpaceState::MovementOnSpline) && MovementSplineComponentFromWorld)
			{
				FVector CharacterWorldLocation = GetRootComponent()->GetComponentLocation();
				FRotator ClosestWorldRotationOnSpline = MovementSplineComponentFromWorld->FindRotationClosestToWorldLocation(FVector{CharacterWorldLocation.X, CharacterWorldLocation.Y, 0.0f}, ESplineCoordinateSpace::World);

				// face/rotate the Character in moving direction, since the FindRotationClosestToWorldLocation() does not account for this
				if (FMath::Abs(ClosestWorldRotationOnSpline.Yaw - GetActorForwardVector().Rotation().Yaw) > 90.0f)
				{
					ClosestWorldRotationOnSpline.Yaw += 180.0f;
				}

				AController *PossessingController = GetController();
				if (PossessingController)
				{
					PossessingController->SetControlRotation(ClosestWorldRotationOnSpline);
				}
			}
		}
	}

	// limit negative Z-Velocity (for better falling/air control, etc.)
	if (CharacterMovementComponent && MovementAttributeSet)
	{
		FVector CharacterVelocity = CharacterMovementComponent->Velocity;
		CharacterVelocity.Z = (CharacterVelocity.Z < 0.0f) ? FMath::Max(CharacterVelocity.Z, MovementAttributeSet->GetMaxFallSpeed()) : CharacterVelocity.Z;
		CharacterMovementComponent->Velocity = CharacterVelocity;
	}
}

UAbilitySystemComponent *AProjectLuxCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AProjectLuxCharacter::PossessedBy(AController *NewController)
{
	Super::PossessedBy(NewController);

	// set the view target of the controller
	{
		AProjectLuxPlayerController *PossessingController = Cast<AProjectLuxPlayerController>(NewController);
		if (PossessingController)
		{
			PossessingController->SetViewTarget(this);
		}
	}

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);

		// remove and add again the default abilities in case of changes
		AbilitySystemComponent->ClearAllAbilities();
		for (TSubclassOf<UGameplayAbility> const &DefaultAbility : DefaultAbilities)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(DefaultAbility, 1, -1, this));
		}

		// initialize AttributeSet by an instant GameplayEffect (which does exactly this)
		{
			FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
			EffectContext.AddSourceObject(this);
			AbilitySystemComponent->ApplyGameplayEffectToSelf(AttributeSetInitEffect.GetDefaultObject(), 1.0f, EffectContext);
		}

		// initialize movement related AttributeSet by an instant GameplayEffect (which does exactly this)
		{
			FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
			EffectContext.AddSourceObject(this);
			AbilitySystemComponent->ApplyGameplayEffectToSelf(MovementAttributeSetInitEffect.GetDefaultObject(), 1.0f, EffectContext);
		}

		// add delegates to attribute changes
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &AProjectLuxCharacter::OnHealthChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(MovementAttributeSet->GetMaxWalkSpeedAttribute()).AddUObject(this, &AProjectLuxCharacter::OnMaxWalkSpeedAttributeChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(MovementAttributeSet->GetJumpZVelocityAttribute()).AddUObject(this, &AProjectLuxCharacter::OnJumpZVelocityAttributeChanged);

		// add delegates to GameplayTag changes
		AbilitySystemComponent->RegisterGameplayTagEvent(DeadTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AProjectLuxCharacter::DeadTagChanged);

		// initialize values which use the Attributes from the related AttributeSet
		UCharacterMovementComponent *CharacterMovementComponent = GetCharacterMovement();
		if (CharacterMovementComponent)
		{
			CharacterMovementComponent->MaxWalkSpeed = MovementAttributeSet->GetMaxWalkSpeed();
			CharacterMovementComponent->JumpZVelocity = MovementAttributeSet->GetJumpZVelocity();
		}
	}
}

void AProjectLuxCharacter::JumpPress()
{
	if (AbilitySystemComponent)
	{
		if (AbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(WallJumpAbilityTag)) == false)
		{
			// block jumping when "movement blocking ability" are active
			// Note: We are using the same tags as for the "move blocking", since they are the same.
			// Note: Also this gameplay feature will be transformed into its own ability in a later feature.
			if (AbilitySystemComponent->HasAnyMatchingGameplayTags(MoveBlockingAbilityTags) == false)
			{
				Jump();
			}
		}
	}
}

void AProjectLuxCharacter::JumpRelease()
{
	StopJumping();
}

void AProjectLuxCharacter::MoveRight(float AxisValue)
{
	AxisValueMoveRight = AxisValue;
}

void AProjectLuxCharacter::MoveUp(float AxisValue)
{
	AxisValueMoveUp = AxisValue;
}

void AProjectLuxCharacter::SprintPress()
{
	if (AbilitySystemComponent)
	{
		UCharacterMovementComponent *CharacterMovementComponent = GetCharacterMovement();
		if (CharacterMovementComponent && !CharacterMovementComponent->IsFalling())
		{
			AbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(SprintAbilityTag));
		}
	}
}

void AProjectLuxCharacter::SprintRelease()
{
	if (AbilitySystemComponent->HasMatchingGameplayTag(SprintAbilityTag))
	{
		FGameplayTagContainer SprintAbilityTags(SprintAbilityTag);
		AbilitySystemComponent->CancelAbilities(&SprintAbilityTags);
	}
}

void AProjectLuxCharacter::DashPress()
{
	if (AbilitySystemComponent)
	{
		// enable canceling of active Dash abilities to allow shorter dashes
		if (AbilitySystemComponent->HasMatchingGameplayTag(DashAbilityTag))
		{
			FGameplayTagContainer DashAbilityTags(DashAbilityTag);
			AbilitySystemComponent->CancelAbilities(&DashAbilityTags);
		}
		else if (AbilitySystemComponent->HasMatchingGameplayTag(DoubleDashAbilityTag))
		{
			FGameplayTagContainer DoubleDashAbilityTags(DoubleDashAbilityTag);
			AbilitySystemComponent->CancelAbilities(&DoubleDashAbilityTags);
		}

		// activate Dash if possible, else try to use the DoubleDash
		if (AbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(DashAbilityTag)) == false)
		{
			AbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(DoubleDashAbilityTag));
		}
	}
}

void AProjectLuxCharacter::QuickStepPress()
{
	if (AbilitySystemComponent)
	{
		UCharacterMovementComponent *CharacterMovementComponent = GetCharacterMovement();
		if (CharacterMovementComponent && !CharacterMovementComponent->IsFalling())
		{
			AbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(QuickStepAbilityTag));
		}
	}
}

void AProjectLuxCharacter::GlidePress()
{
	UCharacterMovementComponent *CharacterMovementComponent = GetCharacterMovement();
	if (AbilitySystemComponent && CharacterMovementComponent && CharacterMovementComponent->IsFalling())
	{
		if (AbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(GlideAbilityTag)))
		{
			// we want to cancel the jump when the player is still holding the jump key, while trying to perform the Glide
			StopJumping();
		}
	}
}

void AProjectLuxCharacter::GlideRelease()
{
	FGameplayTagContainer GlideAbilityTags(GlideAbilityTag);
	if (AbilitySystemComponent->HasAnyMatchingGameplayTags(GlideAbilityTags))
	{
		AbilitySystemComponent->CancelAbilities(&GlideAbilityTags);
	}
}

void AProjectLuxCharacter::AttackPress()
{
	if (AbilitySystemComponent)
	{
		// try to set up combo if attack ability is active and the AnimNotify enabled the combo; else activate the abiltiy
		if (AbilitySystemComponent->HasMatchingGameplayTag(AttackAbilityTag))
		{
			if (bAttackAbilityComboEnabled)
			{
				UAnimInstance *AnimInstance = GetMesh()->GetAnimInstance();
				if (AnimInstance)
				{
					AnimInstance->Montage_JumpToSection(AttackAbilityNextSectionCombo, AnimInstance->GetCurrentActiveMontage());
				}
			}
		}
		else
		{
			AbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(AttackAbilityTag));
		}
	}
}

bool AProjectLuxCharacter::GetWallSlidingFlag() const
{
	return bWallSlidingFlag;
}

EMovementSpaceState AProjectLuxCharacter::GetMovementSpaceState() const
{
	return MovementSpace;
}

void AProjectLuxCharacter::SetMovementSpaceState(EMovementSpaceState State)
{
	PreviousMovementSpace = MovementSpace;
	MovementSpace = State;

	OnMovementSpaceStateChanged();
}

EMovementSpaceState AProjectLuxCharacter::GetPreviousMovementSpaceState() const
{
	return PreviousMovementSpace;
}

const USplineComponent *AProjectLuxCharacter::GetMovementSplineComponent()
{
	return MovementSplineComponentFromWorld;
}

void AProjectLuxCharacter::SetMovementSpline(USplineComponent const *MovementSplineComponent)
{
	MovementSplineComponentFromWorld = MovementSplineComponent;
}

void AProjectLuxCharacter::ActivateAttackAbilityCombo(FName ComboNextSectionName)
{
	bAttackAbilityComboEnabled = true;
	AttackAbilityNextSectionCombo = ComboNextSectionName;
}

void AProjectLuxCharacter::DeactivateAttackAbilityCombo()
{
	bAttackAbilityComboEnabled = false;
	AttackAbilityNextSectionCombo = "COMBODEACTIVATED";
}

bool AProjectLuxCharacter::IsDead()
{
	if (AbilitySystemComponent)
	{
		return AbilitySystemComponent->HasMatchingGameplayTag(DeadTag);
	}
	else
	{
		// if the ASC could not be retrieved we can not evaluate the state and therefore returning False
		return false;
	}
}

void AProjectLuxCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectLuxCharacter::UpdateWallSlidingFlag()
{
	UCharacterMovementComponent *CharacterMovementComponent = GetCharacterMovement();

	if (CharacterMovementComponent && CharacterMovementComponent->IsFalling() && IsTouchingWallForWallSlide() && (CharacterMovementComponent->Velocity.Z <= 0.0f))
	{
		SetWallSlidingFlag(true);
	}
	else
	{
		SetWallSlidingFlag(false);
	}
}

void AProjectLuxCharacter::SetWallSlidingFlag(bool bFlagValue)
{
	bWallSlidingFlag = bFlagValue;

	OnWallSlidingFlagSet();
}

void AProjectLuxCharacter::OnWallSlidingFlagSet()
{
	FGameplayTagContainer WallSlideTags;
	WallSlideTags.AddTag(WallSlideAbilityTag);

	if (GetWallSlidingFlag() == true)
	{
		// activate wall slide ability, if not already activated, since the requirements are fulfilled.
		if (AbilitySystemComponent)
		{
			if (AbilitySystemComponent->HasAnyMatchingGameplayTags(WallSlideTags))
			{
				// the wall slide ability is more of a passive ability and its behavior is following here
				// -> passive means that is interacts with other abilites, but not directly (in the Blueprint) doing anything
				UCharacterMovementComponent *CharacterMovementComponent = GetCharacterMovement();
				if (CharacterMovementComponent)
				{
					AController *PossessingController = GetController();
					if (PossessingController)
					{
						// rotate Character to face towards the negated normal of the wall
						FRotator RotationToFaceWall = (-(LastValidWallSlideHitResult.Normal)).Rotation();
						PossessingController->SetControlRotation(RotationToFaceWall);

						// let the Character stick on the wall
						AActor *WallActor = LastValidWallSlideHitResult.GetActor();
						if (WallActor && (GetAttachParentActor() != WallActor))
						{
							AttachToActor(WallActor, FAttachmentTransformRules{EAttachmentRule::KeepWorld, false});
							CharacterMovementComponent->GravityScale = 0.0f;
							CharacterMovementComponent->Velocity = FVector(0.0f, 0.0f, 0.0f);
							// push character to the wall, so he does not hover in front it
							FHitResult WallPushHitResult{};
							SetActorRelativeLocation(LastValidWallSlideHitResult.Distance * GetActorForwardVector(), true, &WallPushHitResult, ETeleportType::ResetPhysics);
						}
					}
				}
			}
			else
			{
				AbilitySystemComponent->TryActivateAbilitiesByTag(WallSlideTags);
			}
		}
	}
	else
	{
		// we are not wall sliding anymore, so cancel the ability and reset to "normal" movement
		if (AbilitySystemComponent)
		{
			if (AbilitySystemComponent->HasAnyMatchingGameplayTags(WallSlideTags))
			{
				AbilitySystemComponent->CancelAbilities(&WallSlideTags);

				UCharacterMovementComponent *CharacterMovementComponent = GetCharacterMovement();
				if (CharacterMovementComponent)
				{
					// Do not change the GravityScale to its default value, when the player wants to (Double-)Dash,
					// since this will affect the GravityScale change in the (Double-)Dash ability
					FGameplayTagContainer DashAbilityTags;
					DashAbilityTags.AddTag(DashAbilityTag);
					DashAbilityTags.AddTag(DoubleDashAbilityTag);
					if (AbilitySystemComponent->HasAnyMatchingGameplayTags(DashAbilityTags) == false)
					{
						CharacterMovementComponent->GravityScale = DefaultCharacterMovementComponentGravityScale;
					}
				}
				AActor *WallActor = LastValidWallSlideHitResult.GetActor();
				if (WallActor && (GetAttachParentActor() == WallActor))
				{
					DetachFromActor(FDetachmentTransformRules{EDetachmentRule::KeepWorld, false});
				}
			}
		}
	}
}

void AProjectLuxCharacter::OnMovementSpaceStateChanged()
{
	if (MovementSpace != PreviousMovementSpace)
	{
		// restrict the movement of the character
		UCharacterMovementComponent *CharacterMovementComponent = GetCharacterMovement();
		if (CharacterMovementComponent)
		{
			switch (MovementSpace)
			{
			case EMovementSpaceState::MovementIn2D:
				switch (PreviousMovementSpace)
				{
				case EMovementSpaceState::MovementIn2D:
					break;
				case EMovementSpaceState::MovementIn3D:
				case EMovementSpaceState::MovementOnSpline:
					CharacterMovementComponent->SetPlaneConstraintEnabled(true);
					CharacterMovementComponent->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::X);
					break;
				default:
					break;
				}
				break;
			case EMovementSpaceState::MovementIn3D:
			case EMovementSpaceState::MovementOnSpline:
				switch (PreviousMovementSpace)
				{
				case EMovementSpaceState::MovementIn2D:
					CharacterMovementComponent->SetPlaneConstraintEnabled(false);
					break;
				case EMovementSpaceState::MovementIn3D:
				case EMovementSpaceState::MovementOnSpline:
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}
		}

		// call the event of the MovementSpaceState change, so that designers can react to the change
		MovementSpaceStateChanged();
	}
}

void AProjectLuxCharacter::OnHealthChanged(FOnAttributeChangeData const &Data)
{
	HealthChanged(Data.OldValue, Data.NewValue);
}

void AProjectLuxCharacter::OnMaxWalkSpeedAttributeChanged(FOnAttributeChangeData const &Data)
{
	UCharacterMovementComponent *CharacterMovementComponent = GetCharacterMovement();
	if (CharacterMovementComponent)
	{
		CharacterMovementComponent->MaxWalkSpeed = Data.NewValue;
	}
}

void AProjectLuxCharacter::OnJumpZVelocityAttributeChanged(FOnAttributeChangeData const &Data)
{
	UCharacterMovementComponent *CharacterMovementComponent = GetCharacterMovement();
	if (CharacterMovementComponent)
	{
		CharacterMovementComponent->JumpZVelocity = Data.NewValue;
	}
}

void AProjectLuxCharacter::DeadTagChanged(const FGameplayTag, int32 NewCount)
{
	// signal character death on tag application
	if (NewCount == 1)
	{
		Died();
	}
}

TOptional<FHitResult> AProjectLuxCharacter::IsTouchingWallForWallSlide()
{
	FHitResult OutWallHit{};
	FVector LineTraceStart = GetActorLocation();
	FVector LineTraceEnd = LineTraceStart + (GetActorForwardVector() * (GetCapsuleComponent()->GetScaledCapsuleRadius() * 1.5f));
	ECollisionChannel LineTraceChannel{ECollisionChannel::ECC_GameTraceChannel1}; // Wallslide Trace Channel
	FCollisionQueryParams CollisionParams{};
	CollisionParams.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(OutWallHit, LineTraceStart, LineTraceEnd, LineTraceChannel, CollisionParams))
	{
		LastValidWallSlideHitResult = OutWallHit;
		return TOptional<FHitResult>{OutWallHit};
	}
	return TOptional<FHitResult>{};
}

void AProjectLuxCharacter::UpdateMovementToMoveInput()
{
	if (AbilitySystemComponent->HasAnyMatchingGameplayTags(MoveBlockingAbilityTags) || GetWallSlidingFlag())
	{
		return;
	}

	// determine movement direction:
	FVector MovementDirection(0.0f, 0.0f, 0.0f);
	if (AxisValueMoveRight != 0.0f)
	{
		switch (MovementSpace)
		{
		case EMovementSpaceState::MovementIn2D:
		case EMovementSpaceState::MovementIn3D:
			MovementDirection.Y = AxisValueMoveRight;
			break;
		case EMovementSpaceState::MovementOnSpline:
			if (MovementSplineComponentFromWorld)
			{
				MovementDirection = AxisValueMoveRight * MovementSplineComponentFromWorld->FindTangentClosestToWorldLocation(GetRootComponent()->GetComponentLocation(), ESplineCoordinateSpace::World);
			}
			break;
		default:
			break;
		}
	}

	if (AxisValueMoveUp != 0.0f)
	{
		switch (MovementSpace)
		{
		case EMovementSpaceState::MovementIn2D:
		case EMovementSpaceState::MovementIn3D:
			MovementDirection.X = AxisValueMoveUp;
			break;
		case EMovementSpaceState::MovementOnSpline:
			break;
		default:
			break;
		}
	}

	// apply movement if useful:
	if (!MovementDirection.IsNearlyZero())
	{
		MovementDirection.Normalize();
		AddMovementInput(MovementDirection);

		// update last input to match normalization of the movement direction
		switch (MovementSpace)
		{
		case EMovementSpaceState::MovementIn2D:
		case EMovementSpaceState::MovementIn3D:
			AxisValueMoveUp = MovementDirection.X;
			AxisValueMoveRight = MovementDirection.Y;
			break;
		case EMovementSpaceState::MovementOnSpline:
			AxisValueMoveRight = MovementDirection.Y;
			break;
		default:
			break;
		}
	}
}

void AProjectLuxCharacter::UpdateRotationToMoveInput()
{
	UWorld *World = GetWorld();
	UCharacterMovementComponent *CharacterMovementComponent = GetCharacterMovement();
	AController *PossessingController = GetController();

	if (World && CharacterMovementComponent && PossessingController)
	{
		FRotator DesiredRotationFromInput(0.0f, 0.0f, 0.0f);
		float DeltaSeconds = World->GetDeltaSeconds();
		float RotationRateYaw = CharacterMovementComponent->RotationRate.Yaw;
		const bool WallSlideAbilityActive{AbilitySystemComponent->HasAnyMatchingGameplayTags(FGameplayTagContainer(WallSlideAbilityTag))};
		// calculate the desired rotation depending on the input and "movement space state"
		switch (MovementSpace)
		{
		case EMovementSpaceState::MovementIn2D:
			if (AxisValueMoveRight != 0.0f)
			{
				DesiredRotationFromInput = FRotator(0.0f, FMath::RadiansToDegrees(FMath::Atan2(AxisValueMoveRight, 0.0f)), 0.0f);
				if (WallSlideAbilityActive)
				{
					TryRotateAwayFromWall(DesiredRotationFromInput);
				}
				else
				{
					PossessingController->SetControlRotation(FMath::RInterpTo(GetControlRotation(), DesiredRotationFromInput, DeltaSeconds, RotationRateYaw));
				}
			}
			break;
		case EMovementSpaceState::MovementIn3D:
			if ((AxisValueMoveUp != 0.0f) || (AxisValueMoveRight != 0.0f))
			{
				DesiredRotationFromInput = FRotator(0.0f, FMath::RadiansToDegrees(FMath::Atan2(AxisValueMoveRight, AxisValueMoveUp)), 0.0f);
				if (WallSlideAbilityActive)
				{
					TryRotateAwayFromWall(DesiredRotationFromInput);
				}
				else
				{
					PossessingController->SetControlRotation(FMath::RInterpTo(GetControlRotation(), DesiredRotationFromInput, DeltaSeconds, RotationRateYaw));
				}
			}
			break;
		case EMovementSpaceState::MovementOnSpline:
			if ((AxisValueMoveRight != 0.0f) && MovementSplineComponentFromWorld)
			{
				FVector CharacterWorldLocation = GetRootComponent()->GetComponentLocation();
				// we only want to find the closest rotation on the spline in the XY plane, since the Character can move freely in the Z direction
				// Note: we later only need the Yaw value for the rotation
				FRotator ClosestWorldRotationOnSpline = MovementSplineComponentFromWorld->FindRotationClosestToWorldLocation(FVector{CharacterWorldLocation.X, CharacterWorldLocation.Y, 0.0f}, ESplineCoordinateSpace::World);

				// if the Character should go "left" rotate him by 180degrees to face in the left direction
				if (AxisValueMoveRight < 0.0f)
				{
					ClosestWorldRotationOnSpline.Yaw += 180.0f;
				}
				DesiredRotationFromInput = FRotator(0.0f, ClosestWorldRotationOnSpline.Yaw, 0.0f);
				if (WallSlideAbilityActive)
				{
					TryRotateAwayFromWall(DesiredRotationFromInput);
				}
				else
				{
					PossessingController->SetControlRotation(FMath::RInterpTo(GetControlRotation(), DesiredRotationFromInput, DeltaSeconds, RotationRateYaw));
				}
			}
			break;
		default:
			break;
		}
	}
}

void AProjectLuxCharacter::TryRotateAwayFromWall(FRotator3d const &RotationFromInput)
{
	static constexpr double YawAngleToleranceInDegrees{45.0 / 2.0};

	AController *PossessingController = GetController();
	if (PossessingController)
	{
		const double YawAngle{FRotator::NormalizeAxis((RotationFromInput - LastValidWallSlideHitResult.Normal.Rotation()).Yaw)};
		if (FMath::Abs(YawAngle) < YawAngleToleranceInDegrees)
		{
			PossessingController->SetControlRotation(RotationFromInput);
		}
	}
}
