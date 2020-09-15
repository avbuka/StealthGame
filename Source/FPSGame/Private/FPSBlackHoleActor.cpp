// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSBlackHoleActor.h"
//#include "Engine/StaticMeshActor.h"
#include "FPSProjectile.h"
// Sets default values
AFPSBlackHoleActor::AFPSBlackHoleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	RootComponent = MeshComp;

	SmallSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SmallSphereComp"));
	SmallSphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SmallSphereComp->SetGenerateOverlapEvents(true);

	SmallSphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	//SmallSphereComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Overlap);
	SmallSphereComp->SetupAttachment(MeshComp);



	BigSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("BigSphereComp"));
	BigSphereComp->SetupAttachment(MeshComp);
	BigSphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BigSphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);

	

}

// Called when the game starts or when spawned
void AFPSBlackHoleActor::BeginPlay()
{
	Super::BeginPlay();
	SmallSphereComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSBlackHoleActor::OnComponentBeginOverlap);

}

void AFPSBlackHoleActor::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, 
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Somethingoverlaps small sphere"));
	OtherActor->Destroy();
}

// Called every frame
void AFPSBlackHoleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<UPrimitiveComponent*> OverlappingComps;

	BigSphereComp->GetOverlappingComponents(OverlappingComps);
	
	UE_LOG(LogTemp, Warning, TEXT("%d"), OverlappingComps.Num());

	
	for (int32 i = 0; i < OverlappingComps.Num(); i++)
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s"), OverlappingComps[i]->GetFullName());

		if (OverlappingComps[i]->IsSimulatingPhysics())
		{
			FString ErrorLine = "Component simulates physics - %s";
			 ErrorLine.Append(OverlappingComps[i]->GetFullName());

			//UE_LOG(LogTemp, Warning, TEXT("%s"),*ErrorLine);
			OverlappingComps[i]->AddRadialForce(GetActorLocation(), 
				BigSphereComp->GetScaledSphereRadius(), Strengh,
				ERadialImpulseFalloff::RIF_Constant,true);
		} 
		else
		{

		}

		
		
	}

	

}

void AFPSBlackHoleActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	UE_LOG(LogTemp, Error, TEXT("You crossed the Bhole"));

}

