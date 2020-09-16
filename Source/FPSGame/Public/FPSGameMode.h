// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"


#include "FPSGameMode.generated.h"

UCLASS()
class AFPSGameMode : public AGameModeBase
{
	GENERATED_BODY()


protected:

	UPROPERTY(EditDefaultsOnly,Category="Spectating")
	TSubclassOf<AActor> SpectatingViewpoint;

	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
	bool bIsGameWon;


public:



	AFPSGameMode();

	UFUNCTION(BlueprintCallable)
	void CompleteMission(APawn* InitianorPawn, bool bIsMissionSuccess);

	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void OnMissionCompleted(APawn* InitianorPawn, bool bIsMissionSuccess);

	UFUNCTION (BlueprintCallable)
	void FinishingScreen(APawn* InitiatorPawn, bool bIsMissionSuccess);



};



