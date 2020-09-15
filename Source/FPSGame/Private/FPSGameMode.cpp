// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"

AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();
}

void AFPSGameMode::CompleteMission(APawn* InitianorPawn, bool bIsMissionSuccess)
{
	if (InitianorPawn)
	{
		InitianorPawn->DisableInput(nullptr);


		FinishingScreen(InitianorPawn, bIsMissionSuccess);



		OnMissionCompleted(InitianorPawn, bIsMissionSuccess);
	}
}

void AFPSGameMode::FinishingScreen(APawn* InitiatorPawn, bool bIsMissionSuccess)
{

	if (SpectatingViewpoint)
	{
		APlayerController* PlayerController = Cast<APlayerController>(InitiatorPawn->GetController());

		bIsGameWon = bIsMissionSuccess;
		//	AActor* NewViewport;
		TArray<AActor*> ReturnedActors;

		UGameplayStatics::GetAllActorsOfClass(this, SpectatingViewpoint, ReturnedActors);

		if (PlayerController)
		{
			if (ReturnedActors.Num() > 0)
			{
				PlayerController->SetViewTargetWithBlend(ReturnedActors[0], 2.5,
					EViewTargetBlendFunction::VTBlend_Cubic);
			}


		}

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No spectating actors!"));

	}
}
