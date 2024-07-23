#include "BlasterCharacterV2.h"

ABlasterCharacterV2::ABlasterCharacterV2()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABlasterCharacterV2::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABlasterCharacterV2::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABlasterCharacterV2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}