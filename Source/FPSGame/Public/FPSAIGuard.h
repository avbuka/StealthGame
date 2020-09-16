// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/EngineTypes.h"
#include "FPSGameMode.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Engine/TargetPoint.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "Net/UnrealNetwork.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"


#include "FPSAIGuard.generated.h"

class UPawnSensingComponent;
//class UPawnMovementComponent;
UENUM(BlueprintType)
enum class EAIState : uint8
{
	Idle,
	Suspicious,
	Alerted
};

UCLASS()
class FPSGAME_API AFPSAIGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSAIGuard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere,Category="Components")
	UPawnSensingComponent* PawnSensingComp;
	



	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);

	UFUNCTION()
	void OnNoiseHeard(APawn* Initiator, const FVector &location, float Volume);

	UFUNCTION()
	void ResetOrientation();


	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnStateChanged(EAIState NewState);


	UFUNCTION()
	void OnRep_GuardState();

	void SetGuardState(EAIState NewState);

	void MoveToNextPatrolPoint();

	UPROPERTY(ReplicatedUsing=OnRep_GuardState)
	EAIState GuardState;



	FTimerHandle TimerHandleResetOrientation;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "AI", meta =(EditCondition="bIsPatrolling"))
	TArray<ATargetPoint*> TargetPointsArray;

	UPROPERTY(EditInstanceOnly, Category = "AI")
		bool bIsPatrolling;

	
	int8 CurrentTargetPoint;


	UPROPERTY(EditAnywhere, Category = "Components")
	FRotator OriginalRotation;



private:
	void FaceTheNoise(APawn* Pawn, const FVector& location);
	void ChooseNextTargetPoint();
};
