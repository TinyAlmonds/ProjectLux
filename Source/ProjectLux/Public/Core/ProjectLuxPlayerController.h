// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ProjectLuxPlayerController.generated.h"

/** PlayerController of the ProjectLuxCharacter. */
UCLASS()
class PROJECTLUX_API AProjectLuxPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	/** The default constructor of the class.*/
	AProjectLuxPlayerController();

protected:
	/** Method binding to the Input axis/action mappings.*/
	virtual void SetupInputComponent() override;

private:
	/** Method bound to the "Jump" input action mapping, when the button is pressed. Redirects the input to the related method of the ProjectLuxCharacter. */
	void JumpPress();

	/** Method bound to the "Jump" input action mapping, when the button is released. Redirects the input to the related method of the ProjectLuxCharacter. */
	void JumpRelease();

	/** Method bound to the "MoveRight" input axis mapping. Redirects the input to the related method of the ProjectLuxCharacter. */
	void MoveRight(float AxisValue);

	/** Method bound to the "MoveUp" input axis mapping. Redirects the input to the related method of the ProjectLuxCharacter. */
	void MoveUp(float AxisValue);	
};
