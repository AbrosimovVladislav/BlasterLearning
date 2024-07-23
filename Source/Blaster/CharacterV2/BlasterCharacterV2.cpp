#include "BlasterCharacterV2.h"

#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"

ABlasterCharacterV2::ABlasterCharacterV2()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->SetRelativeLocation(FVector(0.f, 0.f, 88.f));
	CameraBoom->SetRelativeRotation(FRotator(-40.f, 0.f, 0.f));
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void ABlasterCharacterV2::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABlasterCharacterV2::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABlasterCharacterV2::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ABlasterCharacterV2::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ABlasterCharacterV2::LookUp);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
}

void ABlasterCharacterV2::BeginPlay()
{
	Super::BeginPlay();
}

void ABlasterCharacterV2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABlasterCharacterV2::MoveForward(float Value)
{
	if (Controller != nullptr && Value != 0.f)
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
		AddMovementInput(Direction, Value);
	}
}

void ABlasterCharacterV2::MoveRight(float Value)
{
	if (Controller != nullptr && Value != 0.f)
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));
		AddMovementInput(Direction, Value);
	}
}

void ABlasterCharacterV2::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void ABlasterCharacterV2::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}
