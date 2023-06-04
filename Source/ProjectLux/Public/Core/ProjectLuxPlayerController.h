// Copyright TinyAlmonds (Alex Noerdemann)
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

	virtual void DisableInput(class APlayerController* PlayerController) override;

private:
	/** Method bound to the "Jump" input action mapping, when the button is pressed. Redirects the input to the related method of the ProjectLuxCharacter. */
	void JumpPress();

	/** Method bound to the "Jump" input action mapping, when the button is released. Redirects the input to the related method of the ProjectLuxCharacter. */
	void JumpRelease();

	/**
	 * Method bound to the "MoveRight" input axis mapping. Redirects the input to the related method of the ProjectLuxCharacter.
	 * @param AxisValue - The current axis value (range: -1.0 to 1.0).
	 */
	void MoveRight(float AxisValue);

	/**
	 * Method bound to the "MoveUp" input axis mapping. Redirects the input to the related method of the ProjectLuxCharacter.
	 * @param AxisValue - The current axis value (range: -1.0 to 1.0).
	 */
	void MoveUp(float AxisValue);

	/** Method bound to the "Sprint" input action mapping, when the button is pressed. Redirects the input to the related method of the ProjectLuxCharacter. */
	void SprintPress();

	/** Method bound to the "Sprint" input action mapping, when the button is released. Redirects the input to the related method of the ProjectLuxCharacter. */
	void SprintRelease();

	/** Method bound to the "Dash" input action mapping, when the button is pressed. Redirects the input to the related method of the ProjectLuxCharacter. */
	void DashPress();

	/** Method bound to the "Glide" input action mapping, when the button is pressed. Redirects the input to the related method of the ProjectLuxCharacter. */
	void GlidePress();

	/** Method bound to the "Attack" input action mapping, when the button is pressed. Redirects the input to the related method of the ProjectLuxCharacter. */
	void AttackPress();

	/** Method bound to the "QuickStep" input action mapping, when the button is pressed. Redirects the input to the related method of the ProjectLuxCharacter. */
	void QuickStepPress();

	/** Method bound to the "ShowPauseMenu" input action mapping, when the button is pressed. Redirects the input to the related method of the ProjectLuxHUD. */
	void ShowPauseMenuPress();
};
