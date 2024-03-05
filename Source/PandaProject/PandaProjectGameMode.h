// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PandaProjectGameMode.generated.h"

UCLASS(minimalapi)
class APandaProjectGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APandaProjectGameMode();
  UFUNCTION(BlueprintImplementableEvent, Category = "Panda")
	  void OnGameOver();

  UFUNCTION(BlueprintImplementableEvent, Category = "Panda")
	  void OnWinGame();
};



