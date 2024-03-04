// Copyright TinyAlmonds (Alex Noerdemann)
#include "Core/Component/ChaseActorAlongSpline/PLChaseActorAlongSplineComponent.h"

#include "Components/SplineComponent.h"
#include "Kismet/GameplayStatics.h"

UPLChaseActorAlongSplineComponent::UPLChaseActorAlongSplineComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPLChaseActorAlongSplineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// We have to query the player on every tick, since the player can die and therefore the actor changes.
	if (ChaseActorSettings.ActorMode == EPLChaseActorAlongSplineActorMode::Player)
	{
		SetChaseActorForPlayerActorMode();
	}

	AActor *OwnerActor = GetOwner();
	switch (ChaseActorSettings.ChaseMode)
	{
	case EPLChaseActorAlongSplineChaseMode::ClosestPointOnChaseSpline:
		if (IsValid(OwnerActor) && ChaseActorSettings.ActorToChase.IsValid(false, false) && SplineToChaseAlong.IsValid(false, false))
		{
			const FVector CurrentClosestPointOnChaseSpline = OwnerActor->GetActorLocation();
			const FVector NewClosestPointOnChaseSpline = SplineToChaseAlong->FindLocationClosestToWorldLocation(ChaseActorSettings.ActorToChase.Get()->GetActorLocation(), ESplineCoordinateSpace::World);
			OwnerActor->SetActorLocation(FMath::VInterpTo(CurrentClosestPointOnChaseSpline, NewClosestPointOnChaseSpline, DeltaTime, ChaseActorSettings.ChaseSpeed));
		}
		break;
	case EPLChaseActorAlongSplineChaseMode::FollowPositionOnReferenceSpline:
		if (IsValid(OwnerActor) && ChaseActorSettings.ActorToChase.IsValid(false, false) && SplineToChaseAlong.IsValid(false, false) && ReferenceSplineToFollow.IsValid(false, false))
		{
			const float NormalizedReferenceSplineInputKey = ReferenceSplineToFollow->FindInputKeyClosestToWorldLocation(ChaseActorSettings.ActorToChase.Get()->GetActorLocation()) / ReferenceSplineToFollow->GetNumberOfSplineSegments();
			const float CurrentInputKeyOnChaseSpline = SplineToChaseAlong->FindInputKeyClosestToWorldLocation(OwnerActor->GetActorLocation());
			const float InputKeyOfReferenceSplineOnChaseSpline = NormalizedReferenceSplineInputKey * SplineToChaseAlong->GetNumberOfSplineSegments();
			const float NewInputKeyOnChaseSpline = FMath::FInterpTo(CurrentInputKeyOnChaseSpline, InputKeyOfReferenceSplineOnChaseSpline, DeltaTime, ChaseActorSettings.ChaseSpeed);
			OwnerActor->SetActorLocation(SplineToChaseAlong->GetLocationAtSplineInputKey(NewInputKeyOnChaseSpline, ESplineCoordinateSpace::World));
		}
		break;
	default:
		break;
	}
}

EPLChaseActorAlongSplineActorMode UPLChaseActorAlongSplineComponent::GetActorMode() const
{
	return ChaseActorSettings.ActorMode;
}

void UPLChaseActorAlongSplineComponent::SetActorMode(EPLChaseActorAlongSplineActorMode Mode)
{
	PreviousActorMode = ChaseActorSettings.ActorMode;
	ChaseActorSettings.ActorMode = Mode;

	if (PreviousActorMode != ChaseActorSettings.ActorMode)
	{
		ActorModeChanged();
	}
}

const AActor *UPLChaseActorAlongSplineComponent::GetChasedActor() const
{
	return ChaseActorSettings.ActorToChase.Get();
}

void UPLChaseActorAlongSplineComponent::SetChaseActor(AActor *ActorToChase)
{
	ChaseActorSettings.ActorToChase = ActorToChase;
}

void UPLChaseActorAlongSplineComponent::BeginPlay()
{
	Super::BeginPlay();

	// Get the SplineComponent of the Actor the Owner is attached to.
	const AActor *OwnerActor = GetOwner();
	if (IsValid(OwnerActor))
	{
		AActor *AttachParentActor = OwnerActor->GetAttachParentActor();
		if (IsValid(AttachParentActor))
		{
			USplineComponent *SplineComponent = AttachParentActor->GetComponentByClass<USplineComponent>();
			if (IsValid(SplineComponent))
			{
				SplineToChaseAlong = SplineComponent;
			}
		}
	}

	// Get the reference SplineComponent for the FollowPositionOnReferenceSpline state.
	if (ChaseActorSettings.ChaseMode == EPLChaseActorAlongSplineChaseMode::FollowPositionOnReferenceSpline)
	{
		if (ChaseActorSettings.ReferenceSplineActor.IsValid(false, false))
		{
			USplineComponent *SplineComponent = ChaseActorSettings.ReferenceSplineActor.Get()->GetComponentByClass<USplineComponent>();
			if (IsValid(SplineComponent))
			{
				ReferenceSplineToFollow = SplineComponent;
			}
		}
	}
}

void UPLChaseActorAlongSplineComponent::ActorModeChanged()
{
	// Invalidate chased actor, since when we change from "Player to Actor" the ActorToChase will not be automatically set.
	ChaseActorSettings.ActorToChase = nullptr;
}

void UPLChaseActorAlongSplineComponent::SetChaseActorForPlayerActorMode()
{
	APawn *Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (Player)
	{
		// Since ACharacter is an AActor it is safe to cast without further checks.
		SetChaseActor(Cast<AActor>(Player));
	}
}
