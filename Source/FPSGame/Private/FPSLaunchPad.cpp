// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSLaunchPad.h"

// Sets default values
AFPSLaunchPad::AFPSLaunchPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	StaticMesh->SetCollisionResponseToAllChannels(ECR_Block);
	
	RootComponent = StaticMesh;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComp->SetCollisionResponseToAllChannels(ECR_Overlap);
	BoxComp->SetupAttachment(StaticMesh);

	BoxComp->SetBoxExtent(FVector(200., 100., 10.));
	

}

void AFPSLaunchPad::HandleOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Something overlaps"));

	FString line = OtherComp->GetFullName();
	
	UE_LOG(LogTemp, Warning, TEXT("%s"),*line);

	

	//if (OtherComp->IsSimulatingPhysics())
	{
		
		ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor);
		

		//UE_LOG(LogTemp, Error, TEXT("Component simulates physics"));


		if (OtherCharacter)
		{
			FRotator Rotator = OtherCharacter->GetViewRotation();
			Rotator.Pitch = 45;
			FVector PlayerVector = Rotator.Vector();

			PlayerVector*= Strength;
			
			OtherCharacter->LaunchCharacter(PlayerVector, true, true);

			UGameplayStatics::SpawnEmitterAtLocation(this, LaunchFX, OtherActor->GetActorLocation());
		}
		else
		{
			line = OtherCharacter->GetFullName();
			
			OtherComp->AddImpulse(LaunchVector,NAME_None,true);
			
			UGameplayStatics::SpawnEmitterAtLocation(this, LaunchFX, OtherActor->GetActorLocation());

		}

	}
}

// Called when the game starts or when spawned
void AFPSLaunchPad::BeginPlay()
{
	Super::BeginPlay();

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSLaunchPad::HandleOverlap);

	
}

// Called every frame
void AFPSLaunchPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

