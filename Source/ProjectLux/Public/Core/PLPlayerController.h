// Copyright TinyAlmonds (Alex Noerdemann)
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "PLPlayerController.generated.h"

// Forward declarations
class UCameraComponent;

/** PlayerController of the PLCharacter. */
UCLASS()
class PROJECTLUX_API APLPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	/** The default constructor of the class.*/
	APLPlayerController();

	/**
	 * Returns the UCameraComponent of the view target.
	 * @return A pointer to the view target's UCameraComponent if found; otherwise a nullptr.
	 */
	UFUNCTION(BlueprintCallable, Category = "PLPlayerController")
	virtual UCameraComponent *GetViewTargetCameraComponent() const;

protected:
	/** Method binding to the Input axis/action mappings.*/
	virtual void SetupInputComponent() override;

	virtual void DisableInput(class APlayerController *PlayerController) override;

private:
	/** Method bound to the "Jump" input action mapping, when the button is pressed. Redirects the input to the related method of the PLCharacter. */
	void JumpPress();

	/** Method bound to the "Jump" input action mapping, when the button is released. Redirects the input to the related method of the PLCharacter. */
	void JumpRelease();

	/**
	 * Method bound to the "MoveRight" input axis mapping. Redirects the input to the related method of the PLCharacter.
	 * @param AxisValue - The current axis value (range: -1.0 to 1.0).
	 */
	void MoveRight(float AxisValue);

	/**
	 * Method bound to the "MoveUp" input axis mapping. Redirects the input to the related method of the PLCharacter.
	 * @param AxisValue - The current axis value (range: -1.0 to 1.0).
	 */
	void MoveUp(float AxisValue);

	/** Method bound to the "Sprint" input action mapping, when the button is pressed. Redirects the input to the related method of the PLCharacter. */
	void SprintPress();

	/** Method bound to the "Sprint" input action mapping, when the button is released. Redirects the input to the related method of the PLCharacter. */
	void SprintRelease();

	/** Method bound to the "Dash" input action mapping, when the button is pressed. Redirects the input to the related method of the PLCharacter. */
	void DashPress();

	/** Method bound to the "Glide" input action mapping, when the button is pressed. Redirects the input to the related method of the PLCharacter. */
	void GlidePress();

	/** Method bound to the "Attack" input action mapping, when the button is pressed. Redirects the input to the related method of the PLCharacter. */
	void AttackPress();

	/** Method bound to the "QuickStep" input action mapping, when the button is pressed. Redirects the input to the related method of the PLCharacter. */
	void QuickStepPress();

	/** Method bound to the "ShowPauseMenu" input action mapping, when the button is pressed. Redirects the input to the related method of the PLHUD. */
	void ShowPauseMenuPress();
};
