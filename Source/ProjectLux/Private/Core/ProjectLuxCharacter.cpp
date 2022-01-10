#include "Core/ProjectLuxCharacter.h"
#include "Core/ProjectLuxCharacterAttributeSet.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SplineComponent.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayEffectTypes.h"
#include "Math/UnrealMathUtility.h"
#include "Misc/Optional.h"


AProjectLuxCharacter::AProjectLuxCharacter() : 
	AxisValueMoveUp{0.0f},
	AxisValueMoveRight{0.0f},
	bWallSlidingFlag{false},
	MovementSpace{EMovementSpaceState::MovementIn3D},
	PreviousMovementSpace{EMovementSpaceState::MovementIn3D},
	MovementSplineComponentFromWorld{nullptr},
	WallSlideAbilityTag{ FGameplayTag::RequestGameplayTag(FName("Ability.Movement.WallSlide")) },
	WallJumpAbilityTag{ FGameplayTag::RequestGameplayTag(FName("Ability.Movement.WallJump")) },
	DashAbilityTag{ FGameplayTag::RequestGameplayTag(FName("Ability.Movement.Dash")) },
	DoubleDashAbilityTag{FGameplayTag::RequestGameplayTag(FName("Ability.Movement.DoubleDash"))},
	AttackAbilityTag{ FGameplayTag::RequestGameplayTag(FName("Ability.Combat.Attack")) }
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Construct the ASC
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	// Construct the attribute set
	AttributeSet = CreateDefaultSubobject<UProjectLuxCharacterAttributeSet>(TEXT("AttributeSet"));

	// Fill the FGameplayTagContainer which blocking certain inputs/abilities
	MoveBlockingAbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Reject.MoveInput")));
	MoveBlockingAbilityTags.AddTag(DashAbilityTag);
	MoveBlockingAbilityTags.AddTag(DoubleDashAbilityTag);

	// Character Settings
	VelocityZWallSlide = DefaultValues.VelocityZWallSlide;
	VelocityXYMultiplierWallJump = DefaultValues.VelocityXYMultiplierWallJump;
	VelocityZMultiplierWallJump = DefaultValues.VelocityZMultiplierWallJump;
	VelocityMultiplierDash = DefaultValues.VelocityMultiplierDash;
	JumpMaxHoldTime = DefaultValues.CharacterJumpMaxHoldTime;

	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	if (CharacterMovementComponent)
	{
		// General Settings
		CharacterMovementComponent->GravityScale = DefaultValues.CharacterMovementComponentGravityScale;
		CharacterMovementComponent->MaxAcceleration = DefaultValues.CharacterMovementComponentMaxAcceleration;

		// Walking Settings
		CharacterMovementComponent->MaxWalkSpeed = DefaultValues.CharacterMovementComponentMaxWalkSpeed;
		CharacterMovementComponent->GroundFriction = DefaultValues.CharacterMovementComponentGroundFriction;

		// Jumping/Falling Settings
		CharacterMovementComponent->JumpZVelocity = DefaultValues.CharacterMovementComponentJumpZVelocity;
		CharacterMovementComponent->BrakingDecelerationFalling = DefaultValues.CharacterMovementComponentBrakingDecelerationFalling;
		CharacterMovementComponent->AirControl = DefaultValues.CharacterMovementComponentAirControl;
		CharacterMovementComponent->AirControlBoostMultiplier = DefaultValues.CharacterMovementComponentAirControlBoostMultiplier;
		CharacterMovementComponent->AirControlBoostVelocityThreshold = DefaultValues.CharacterMovementComponentAirControlBoostVelocityThreshold;

		// Rotation Settings
		CharacterMovementComponent->RotationRate = DefaultValues.CharacterMovementComponentRotationRate;
	}
}

void AProjectLuxCharacter::PostInitProperties()
{
	Super::PostInitProperties();

	// Update default values with properties
	// Character Settings
	DefaultValues.VelocityZWallSlide = VelocityZWallSlide;
	DefaultValues.VelocityXYMultiplierWallJump = VelocityXYMultiplierWallJump;
	DefaultValues.VelocityZMultiplierWallJump= VelocityZMultiplierWallJump;
	DefaultValues.VelocityMultiplierDash = VelocityMultiplierDash;
	DefaultValues.CharacterJumpMaxHoldTime = JumpMaxHoldTime;

	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	if (CharacterMovementComponent)
	{
		// General Settings
		DefaultValues.CharacterMovementComponentGravityScale = CharacterMovementComponent->GravityScale;
		DefaultValues.CharacterMovementComponentMaxAcceleration = CharacterMovementComponent->MaxAcceleration;

		// Walking Settings
		DefaultValues.CharacterMovementComponentMaxWalkSpeed = CharacterMovementComponent->MaxWalkSpeed;
		DefaultValues.CharacterMovementComponentGroundFriction = CharacterMovementComponent->GroundFriction;

		// Jumping/Falling Settings
		DefaultValues.CharacterMovementComponentJumpZVelocity = CharacterMovementComponent->JumpZVelocity;
		DefaultValues.CharacterMovementComponentBrakingDecelerationFalling = CharacterMovementComponent->BrakingDecelerationFalling;
		DefaultValues.CharacterMovementComponentAirControl = CharacterMovementComponent->AirControl;
		DefaultValues.CharacterMovementComponentAirControlBoostMultiplier = CharacterMovementComponent->AirControlBoostMultiplier;
		DefaultValues.CharacterMovementComponentAirControlBoostVelocityThreshold = CharacterMovementComponent->AirControlBoostVelocityThreshold;

		// Rotation Settings
		DefaultValues.CharacterMovementComponentRotationRate = CharacterMovementComponent->RotationRate;
	}
}


void AProjectLuxCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
			UpdateRotationToMoveInput();
		}
		else
		{
			// stop jumping, when "movement blocking abilities" are active
			UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
			if (CharacterMovementComponent)
			{
				if (CharacterMovementComponent->IsFalling())
				{
					StopJumping();
				}
			}

			// rotate Character while dashing on a Spline
			if ((MovementSpace == EMovementSpaceState::MovementOnSpline) && MovementSplineComponentFromWorld)
			{
				FVector CharacterWorldLocation = GetRootComponent()->GetComponentLocation();
				FRotator ClosestWorldRotationOnSpline = MovementSplineComponentFromWorld->FindRotationClosestToWorldLocation(FVector{ CharacterWorldLocation.X, CharacterWorldLocation.Y, 0.0f }, ESplineCoordinateSpace::World);

				// face/rotate the Character in Dash direction, since the FindRotationClosestToWorldLocation() does not account for this
				if (FMath::Abs(ClosestWorldRotationOnSpline.Yaw - GetActorForwardVector().Rotation().Yaw) > 90.0f)
				{
					ClosestWorldRotationOnSpline.Yaw += 180.0f;
				}

				AController* PossessingController = GetController();
				if (PossessingController)
				{
					PossessingController->SetControlRotation(ClosestWorldRotationOnSpline);
				}
			}
		}
	}

	// limit negative Z-Velocity (for better falling/air control, etc.)
	{
		UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
		if (CharacterMovementComponent)
		{
			FVector CharacterVelocity = CharacterMovementComponent->Velocity;
			CharacterVelocity.Z = (CharacterVelocity.Z < 0.0f) ? FMath::Max(CharacterVelocity.Z, -CharacterMovementComponent->MaxWalkSpeed * VelocityMultiplierDash / 1.0f) : CharacterVelocity.Z;
			CharacterMovementComponent->Velocity = CharacterVelocity;
		}
	}
}

UAbilitySystemComponent* AProjectLuxCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AProjectLuxCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);

		// remove and add again the default abilities in case of changes
		AbilitySystemComponent->ClearAllAbilities();
		for (TSubclassOf<UGameplayAbility> const & DefaultAbility : DefaultAbilities)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(DefaultAbility, 1, -1, this));
		}
		
		// initialize AttributeSet by an instant GameplayEffect (which does exactly this)
		{
			FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
			EffectContext.AddSourceObject(this);
			AbilitySystemComponent->ApplyGameplayEffectToSelf(AttributeSetInitEffect.GetDefaultObject(), 1.0f, EffectContext);
		}

		// add delegates to attribute changes
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &AProjectLuxCharacter::OnHealthChanged);
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

	if (AbilitySystemComponent)
	{
		// block movement when "movement blocking ability" are active or we are wall sliding
		if (AbilitySystemComponent->HasAnyMatchingGameplayTags(MoveBlockingAbilityTags) || GetWallSlidingFlag())
		{
			return;
		}
	}

	if (AxisValue != 0.0f)
	{
		FVector MovementDirection(0.0f, 0.0f, 0.0f);
		switch (MovementSpace)
		{
		case EMovementSpaceState::MovementIn2D:
		case EMovementSpaceState::MovementIn3D:
			MovementDirection.Y = AxisValue;
			AddMovementInput(MovementDirection);
			break;
		case EMovementSpaceState::MovementOnSpline:
			if (MovementSplineComponentFromWorld)
			{
				MovementDirection = AxisValue * MovementSplineComponentFromWorld->FindTangentClosestToWorldLocation(GetRootComponent()->GetComponentLocation(), ESplineCoordinateSpace::World);
				AddMovementInput(MovementDirection);
			}
			break;
		default:
			break;
		}
	}
}

void AProjectLuxCharacter::MoveUp(float AxisValue)
{
	AxisValueMoveUp = AxisValue;

	if (AbilitySystemComponent)
	{
		// block movement when "movement blocking ability" are active or we are wall sliding
		if (AbilitySystemComponent->HasAnyMatchingGameplayTags(MoveBlockingAbilityTags) || GetWallSlidingFlag())
		{
			return;
		}
	}

	if (AxisValue != 0.0f)
	{
		FVector MovementDirection(0.0f, 0.0f, 0.0f);
		switch (MovementSpace)
		{
		case EMovementSpaceState::MovementIn2D:
			break;
		case EMovementSpaceState::MovementIn3D:
			MovementDirection.X = AxisValue;
			AddMovementInput(MovementDirection);
			break;
		case EMovementSpaceState::MovementOnSpline:
			break;
		default:
			break;
		}
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
		if(AbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(DashAbilityTag)) == false)
		{
			AbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(DoubleDashAbilityTag));
		}
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
				UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
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

void AProjectLuxCharacter::SetMovementSpline(USplineComponent const* MovementSplineComponent)
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

void AProjectLuxCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectLuxCharacter::UpdateWallSlidingFlag()
{
	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();

	if (CharacterMovementComponent)
	{
		if (CharacterMovementComponent->IsFalling())
		{
			if (IsTouchingWallForWallSlide())
			{
				SetWallSlidingFlag(true);
			}
			else
			{
				SetWallSlidingFlag(false);
			}
		}
		else
		{
			SetWallSlidingFlag(false);
		}
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
				UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
				if (CharacterMovementComponent)
				{
					AController* PossessingController = GetController();
					if (PossessingController)
					{
						// rotate Character to face towards the negated normal of the wall
						FRotator RotationToFaceWall = (-(LastValidWallSlideHitResult.Normal)).Rotation();
						PossessingController->SetControlRotation(RotationToFaceWall);

						// let the Character slide down the wall on the specified velocity
						CharacterMovementComponent->Velocity = FVector(0.0f, 0.0f, VelocityZWallSlide);
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
		// we are not wall sliding anymore, so cancel the ability
		if (AbilitySystemComponent)
		{
			AbilitySystemComponent->CancelAbilities(&WallSlideTags);
		}
	}
}

void AProjectLuxCharacter::OnMovementSpaceStateChanged()
{
	if (MovementSpace != PreviousMovementSpace)
	{
		// restrict the movement of the character
		UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
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

void AProjectLuxCharacter::WallJump()
{
	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	AController* PossessingController = GetController();
	if (CharacterMovementComponent && PossessingController)
	{
		// Launch Character in opposite direction of its Forward Vector with the specified velocity
		FVector LaunchDirection = -GetActorForwardVector();
		FVector LaunchVelocity = LaunchDirection * (CharacterMovementComponent->MaxWalkSpeed * VelocityXYMultiplierWallJump);
		LaunchVelocity.Z = CharacterMovementComponent->JumpZVelocity * VelocityZMultiplierWallJump;

		LaunchCharacter(LaunchVelocity, false, true);

		// Rotate Character to launch direction
		PossessingController->SetControlRotation(LaunchDirection.Rotation());
	}
}

void AProjectLuxCharacter::Dash()
{
	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	AController* PossessingController = GetController();
	if (CharacterMovementComponent && PossessingController)
	{
		// determine dash direction -and rotation and normalize the direction vector:
		// when "wall sliding" dash horizontally from the wall, else determine dash direction from input
		FVector DashDirection(0.0f, 0.0f, 0.0f);
		FRotator DashRotation(0.0f, 0.0f, 0.0f);
		if (bWallSlidingFlag == true)
		{
			DashDirection = -GetActorForwardVector();
			DashRotation = DashDirection.Rotation();
		}
		else
		{
			switch (MovementSpace)
			{
			case EMovementSpaceState::MovementIn3D:
				// we still want to dash even if no input is given (in this case dash in forward direction of the character; else in the direction of the input)
				if ((AxisValueMoveUp == 0.0f) && (AxisValueMoveRight == 0.0f))
				{
					DashDirection = GetActorForwardVector();
					DashRotation = DashDirection.Rotation();
				}
				else
				{
					DashDirection = FVector(AxisValueMoveUp, AxisValueMoveRight, 0.0f);
					DashRotation = DashDirection.Rotation();
				}
				break;
			case EMovementSpaceState::MovementIn2D:
			case EMovementSpaceState::MovementOnSpline:
				// we still want to dash even if no input is given (in this case dash in forward direction of the character; else in the direction of the input)
				if ((AxisValueMoveUp == 0.0f) && (AxisValueMoveRight == 0.0f))
				{
					DashDirection = GetActorForwardVector();
					DashRotation = DashDirection.Rotation();
				}
				// we don't rotate the Character if we only dash upwards/downwards
				else if (AxisValueMoveRight == 0.0f)
				{
					DashDirection = FVector(0.0f, AxisValueMoveRight, AxisValueMoveUp);
					DashRotation = GetActorForwardVector().Rotation();
				}
				// rotate to input in all other cases
				else
				{
					DashDirection = FVector(0.0f, AxisValueMoveRight, AxisValueMoveUp);
					DashRotation = DashDirection.Rotation();
				}				
				break;
			default:
				return;
				break;
			}
		}
		DashDirection.Normalize();

		// change some values on the CharacterMovementComponent, so that the dash is the same on the ground as in the air
		CharacterMovementComponent->GroundFriction = 0.0f;
		CharacterMovementComponent->GravityScale = 0.0f;

		// perform the dash in the direction of its velocity vector
		FVector DashVelocityDirection = DashDirection * (CharacterMovementComponent->MaxWalkSpeed * VelocityMultiplierDash);
		LaunchCharacter(DashVelocityDirection, true, true);

		// rotate Character to dash direction but only around Yaw axis
		DashRotation = FRotator(0.0f, DashRotation.Yaw, 0.0f);
		PossessingController->SetControlRotation(DashRotation);
	}
}

void AProjectLuxCharacter::StopDash()
{
	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	if (CharacterMovementComponent)
	{
		CharacterMovementComponent->GroundFriction = DefaultValues.CharacterMovementComponentGroundFriction;
		CharacterMovementComponent->GravityScale = DefaultValues.CharacterMovementComponentGravityScale;
		CharacterMovementComponent->Velocity = FVector(0.0f, 0.0f, 0.0f);
	}
}

void AProjectLuxCharacter::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	HealthChanged(Data.OldValue, Data.NewValue);
}

TOptional<FHitResult> AProjectLuxCharacter::IsTouchingWallForWallSlide()
{
	FHitResult OutWallHit;
	FVector LineTraceStart = GetActorLocation();
	FVector LineTraceEnd = LineTraceStart + (GetActorForwardVector() * (GetCapsuleComponent()->GetScaledCapsuleRadius() * 1.5f));
	FName LineTraceProfileName = FName(TEXT("IgnoreOnlyPawn"));
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByProfile(OutWallHit, LineTraceStart, LineTraceEnd, LineTraceProfileName, CollisionParams))
	{
		LastValidWallSlideHitResult = OutWallHit;
		return TOptional<FHitResult>{OutWallHit};
	}
	return TOptional<FHitResult>{};
}

void AProjectLuxCharacter::UpdateRotationToMoveInput()
{
	UWorld* World = GetWorld();
	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	AController* PossessingController = GetController();

	if (World && CharacterMovementComponent && PossessingController)
	{
		FRotator DesiredRotationFromInput(0.0f, 0.0f, 0.0f);
		float DeltaSeconds = World->GetDeltaSeconds();
		float RotationRateYaw = CharacterMovementComponent->RotationRate.Yaw;
		// calculate the desired rotation depending on the input and "movement space state"
		switch (MovementSpace)
		{
		case EMovementSpaceState::MovementIn2D:
			if (AxisValueMoveRight != 0.0f)
			{
				DesiredRotationFromInput = FRotator(0.0f, FMath::RadiansToDegrees(FMath::Atan2(AxisValueMoveRight, 0.0f)), 0.0f);
				PossessingController->SetControlRotation(FMath::RInterpTo(GetControlRotation(), DesiredRotationFromInput, DeltaSeconds, RotationRateYaw));
			}
			break;
		case EMovementSpaceState::MovementIn3D:
			if ((AxisValueMoveUp != 0.0f) || (AxisValueMoveRight != 0.0f))
			{
				DesiredRotationFromInput = FRotator(0.0f, FMath::RadiansToDegrees(FMath::Atan2(AxisValueMoveRight, AxisValueMoveUp)), 0.0f);
				PossessingController->SetControlRotation(FMath::RInterpTo(GetControlRotation(), DesiredRotationFromInput, DeltaSeconds, RotationRateYaw));
			}
			break;
		case EMovementSpaceState::MovementOnSpline:
			if ((AxisValueMoveRight != 0.0f) && MovementSplineComponentFromWorld)
			{
				FVector CharacterWorldLocation = GetRootComponent()->GetComponentLocation();
				// we only want to find the closest rotation on the spline in the XY plane, since the Character can move freely in the Z direction
				// Note: we later only need the Yaw value for the rotation
				FRotator ClosestWorldRotationOnSpline = MovementSplineComponentFromWorld->FindRotationClosestToWorldLocation(FVector{ CharacterWorldLocation.X, CharacterWorldLocation.Y, 0.0f }, ESplineCoordinateSpace::World);

				// if the Character should go "left" rotate him by 180degrees to face in the left direction
				if (AxisValueMoveRight < 0.0f)
				{
					ClosestWorldRotationOnSpline.Yaw += 180.0f;
				}
				DesiredRotationFromInput = FRotator(0.0f, ClosestWorldRotationOnSpline.Yaw, 0.0f);
				PossessingController->SetControlRotation(FMath::RInterpTo(GetControlRotation(), DesiredRotationFromInput, DeltaSeconds, RotationRateYaw));
			}
			break;
		default:
			break;
		}
	}	
}
