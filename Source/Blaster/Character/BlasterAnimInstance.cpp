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
	FRotator Rotation = BlasterCharacter->GetActorRotation();
	FVector ForwardVector = Rotation.Vector();
	FVector RightVector = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);

	ForwardSpeed = FVector::DotProduct(ForwardVector, Velocity);
	RightSpeed = FVector::DotProduct(RightVector, Velocity);

	bIsInAir = BlasterCharacter->GetCharacterMovement()->IsFalling();

	bIsAccelerating = BlasterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f;

	// Debug message to ensure values are being updated correctly
	GEngine->AddOnScreenDebugMessage(-1, 0.05f, FColor::Purple,
	                                 FString::Printf(
		                                 TEXT("ForwardSpeed: %f, RightSpeed: %f"), ForwardSpeed, RightSpeed));
}
