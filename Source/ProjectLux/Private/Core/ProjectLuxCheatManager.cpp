// Copyright TinyAlmonds (Alex Noerdemann)

#include "Core/ProjectLuxCheatManager.h"

void UProjectLuxCheatManager::TeleportToPlayerStart_Implementation(FName PlayerStartTag)
{
    static_cast<void>(PlayerStartTag);
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("TeleportToPlayerStart command not implemented. Expecting implementation in Blueprint."));
}
