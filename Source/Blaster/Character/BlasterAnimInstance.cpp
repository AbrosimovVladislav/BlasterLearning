// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterAnimInstance.h"
#include "BlasterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UBlasterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	BlasterCharacter = Cast<ABlasterCharacter>(TryGetPawnOwner());
}

void UBlasterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (BlasterCharacter == nullptr)
	{
		BlasterCharacter = Cast<ABlasterCharacter>(TryGetPawnOwner());
	}
	if (BlasterCharacter == nullptr) return;

	FVector Velocity = BlasterCharacter->GetVelocity();
	FVector ForwardVector = BlasterCharacter->GetForwardVector();
	FVector RightVector = BlasterCharacter->GetRightVector();

	
	GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Purple,
		FString::Printf(TEXT("Fwd: %s, Rght: %s"), *ForwardVector.ToString(), *RightVector.ToString()));
	
	ForwardSpeed = FVector::DotProduct(ForwardVector, Velocity);
	RightSpeed = FVector::DotProduct(RightVector, Velocity);

	bIsInAir = BlasterCharacter->GetCharacterMovement()->IsFalling();

	bIsAccelerating = BlasterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f;
}
