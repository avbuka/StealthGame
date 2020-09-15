// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAIGuard.h"

// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
	
		TargetPointsArray.Add(CreateDefaultSubobject<ATargetPoint>(TEXT("TargetPoint1")));
		TargetPointsArray.Add(CreateDefaultSubobject<ATargetPoint>(TEXT("TargetPoint2")));
		TargetPointsArray.Add(CreateDefaultSubobject<ATargetPoint>(TEXT("TargetPoint3")));
		TargetPointsArray.Add(CreateDefaultSubobject<ATargetPoint>(TEXT("TargetPoint4")));
		TargetPointsArray.Add(CreateDefaultSubobject<ATargetPoint>(TEXT("TargetPoint5")));
		///SecondTargetPoint = CreateDefaultSubobject<ATargetPoint>(TEXT("SecondTargetPoint2"));
	
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnNoiseHeard);

	OriginalRotation = GetActorRotation();

	GuardState = EAIState::Idle;

	if (bIsPatrolling)
	{
		MoveToNextPatrolPoint();

	}

}

void AFPSAIGuard::OnPawnSeen(APawn* SeenPawn)
{
	DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.f, 12, FColor::Black, false, 10.f);

	SetGuardState(EAIState::Alerted);
	
	
	AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
	GM->CompleteMission(SeenPawn,false);

	
}




void AFPSAIGuard::OnNoiseHeard(APawn* Inititiator, const FVector& location, float Volume)
{

	if (GuardState == EAIState::Alerted)
	{
		return;
	}
	
	UPawnMovementComponent* PawnMovementComponent = this->GetMovementComponent();
	
	PawnMovementComponent->StopActiveMovement();

	
	
	DrawDebugSphere(GetWorld(), location, 32.f, 12, FColor::Green, false, 10.f);


	GetWorldTimerManager().ClearTimer(TimerHandleResetOrientation);
	GetWorldTimerManager().SetTimer(TimerHandleResetOrientation, this,
		&AFPSAIGuard::ResetOrientation, 1.f);


	FaceTheNoise(this, location);

	SetGuardState(EAIState::Suspicious);
	
}




void AFPSAIGuard::ResetOrientation()
{
	if (GuardState == EAIState::Alerted)
	{
		return;
	}

	SetGuardState(EAIState::Idle);

	SetActorRotation(OriginalRotation);

	if(bIsPatrolling)
		MoveToNextPatrolPoint();
}




void AFPSAIGuard::SetGuardState(EAIState NewState)
{

	UE_LOG(LogTemp, Warning, TEXT("Checking"));
	if (GuardState == NewState)
	{
		return;
	}

	if (NewState <= GuardState)
	{
		return;
	}

	UE_LOG(LogTemp, Error, TEXT("New state"));

	GuardState = NewState;

	OnStateChanged(GuardState);
}

void AFPSAIGuard::MoveToNextPatrolPoint()
{

	
	if (CurrentTargetPoint >= TargetPointsArray.Num())
	{
		return;
	}

	

	//AActor ACurrentTarget = Cast<AActor>(TargetPointsArray[CurrentTargetPoint]);


	ATargetPoint *ACurrentPoint = TargetPointsArray[CurrentTargetPoint];
	FVector FCurrentVector = ACurrentPoint->GetActorLocation();

	//UNavigationSystemV1::SimpleMoveToLocation(GetController(),FCurrentVector);
		
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), FCurrentVector);


}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsPatrolling)
	{
		FVector FDelta = GetActorLocation() - TargetPointsArray[CurrentTargetPoint]->GetActorLocation();

		FDelta.Z = 0;
		float Delta = FDelta.Size();

		//UE_LOG(LogTemp, Warning, TEXT("%f"),Delta);


		if (Delta<70)
		{
			ChooseNextTargetPoint();
		}
	}

}

void AFPSAIGuard::FaceTheNoise(APawn* Pawn, const FVector& location)
{
	
	
	
	FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(Pawn->GetActorLocation(),location);
	NewRotation.Pitch = 0;
	NewRotation.Roll = 0;

	Pawn->SetActorRotation(NewRotation,ETeleportType::None);
}

void AFPSAIGuard::ChooseNextTargetPoint()

{
	int Delta = 1;

	if (CurrentTargetPoint == 4 )
	{
		CurrentTargetPoint = 0;

		UE_LOG(LogTemp, Warning, TEXT("Lap"));

		MoveToNextPatrolPoint();
		
		return;
	}

	CurrentTargetPoint += Delta;
	UE_LOG(LogTemp, Warning, TEXT("Next point is %d"),CurrentTargetPoint);

	MoveToNextPatrolPoint();
}

