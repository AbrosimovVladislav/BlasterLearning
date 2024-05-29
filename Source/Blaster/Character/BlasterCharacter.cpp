#include "BlasterCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

ABlasterCharacter::ABlasterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SetReplicates(true);

	CameraSetup();
	TestTextWidgetSetup();
}

// ---Setups---
void ABlasterCharacter::CameraSetup()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeLocation(FVector(0.f, 0.f, 130.f));
	CameraBoom->SetRelativeRotation(FRotator(-40.f, 0.f, 0.f));
	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritRoll = false;
	CameraBoom->bInheritYaw = false;
	CameraBoom->bUsePawnControlRotation = false;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void ABlasterCharacter::TestTextWidgetSetup()
{
	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(RootComponent);
}

void ABlasterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABlasterCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABlasterCharacter::MoveRight);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
}

void ABlasterCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABlasterCharacter, IsEquipped);
}

// ---Begin And Tick---
void ABlasterCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* BlasterCharacterController = Cast<APlayerController>(GetController());
	if (BlasterCharacterController)
	{
		BlasterCharacterController->bShowMouseCursor = true;
	}
}

void ABlasterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RotateCharacterToMouseCursor();
}

// ---Movement---
void ABlasterCharacter::MoveForward(float Value)
{
	if (Controller != nullptr && Value != 0.f)
	{
		const FRotator Rotation = FollowCamera->GetComponentRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ABlasterCharacter::MoveRight(float Value)
{
	if (Controller != nullptr && Value != 0.f)
	{
		const FRotator Rotation = FollowCamera->GetComponentRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

// ---Rotation---
void ABlasterCharacter::RotateCharacterToMouseCursor()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		FVector2D MousePosition;
		if (PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y))
		{
			FRotator NewRotation = DefineRotationByMousePosition(MousePosition, PlayerController);
			PlayerController->SetControlRotation(NewRotation);
		}
	}
}

FRotator ABlasterCharacter::DefineRotationByMousePosition(FVector2D MousePosition,
                                                          APlayerController* PlayerController) const
{
	FVector WorldDirection;
	FVector WorldLocation;
	PlayerController->DeprojectScreenPositionToWorld(
		MousePosition.X, MousePosition.Y, WorldLocation, WorldDirection);
	FVector TargetLocation = WorldLocation + WorldDirection * 1000;
	FRotator FullNewRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation);
	return FRotator(0.0f, FullNewRotation.Yaw, 0.0f);
}

void ABlasterCharacter::RotateCameraToCharacterBack()
{
	FRotator Rotation = GetActorRotation();
	CameraBoom->SetRelativeRotation(FRotator(-40.f, Rotation.Yaw, 0.f));

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		int32 ScreenWidth, ScreenHeight;
		PlayerController->GetViewportSize(ScreenWidth, ScreenHeight);

		// Вычисляем координаты X и Y
		float X = ScreenWidth / 2.0f;
		float Y = ScreenHeight / 4.0f; // Половина между серединой и верхней точкой экрана

		PlayerController->SetMouseLocation(X, Y);
	}
}

// ---Equipping---
void ABlasterCharacter::ChangeEquipped()
{
	IsEquipped = !IsEquipped;
	// if (HasAuthority())
	// {
		// IsEquipped = !IsEquipped;
	// }
	// else
	// {
		// ServerSetIsEquipped(!IsEquipped);
	// }
}

void ABlasterCharacter::ServerSetIsEquipped_Implementation(bool NewValue)
{
	IsEquipped = NewValue;
}

bool ABlasterCharacter::ServerSetIsEquipped_Validate(bool NewValue)
{
	return true;
}

bool ABlasterCharacter::GetIsEquipped() const
{
	return IsEquipped;
}
