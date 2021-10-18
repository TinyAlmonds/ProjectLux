// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ProjectLuxCharacter.generated.h"

// Forward declarations
template<typename OptionalType>
struct TOptional;
struct FHitResult;

UCLASS()
class PROJECTLUX_API AProjectLuxCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// TODO: add documentation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Movement")
	float VelocityZWallSlide;

	// Sets default values for this character's properties
	AProjectLuxCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// TODO: add documentation
	virtual void JumpPress();

	// TODO: add documentation
	virtual void JumpRelease();

	// TODO: add documentation
	virtual void MoveRight(float AxisValue);

	// TODO: add documentation
	virtual void MoveUp(float AxisValue);

	// TODO: add documentation
	virtual bool GetWallSlidingFlag() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// TODO: add documentation
	virtual void UpdateWallSlidingFlag();

	// TODO: add documentation
	virtual void SetWallSlidingFlag(bool bFlagValue);

	// TODO: add documentation
	virtual void OnWallSlidingFlagChanged();

private:
	// TODO: add documentation
	bool bWallSlidingFlag;

	// TODO: add documentation
	TOptional<FHitResult> IsTouchingWallForWallSlide() const;

};
