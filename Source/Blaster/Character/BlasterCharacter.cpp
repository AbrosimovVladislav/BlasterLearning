#include "BlasterCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

ABlasterCharacter::ABlasterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

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
			SetActorRotation(FRotator(0.0f, NewRotation.Yaw, 0.0f));

			ForwardVector = GetActorForwardVector();
			RightVector = GetActorRightVector();
		}
	}
}

FRotator ABlasterCharacter::DefineRotationByMousePosition(FVector2D MousePosition, APlayerController* PlayerController)
{
	FVector WorldDirection;
	FVector WorldLocation;
	PlayerController->DeprojectScreenPositionToWorld(
		MousePosition.X, MousePosition.Y, WorldLocation, WorldDirection);
	FVector TargetLocation = WorldLocation + WorldDirection * 1000;
	return UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation);
}

void ABlasterCharacter::RotateCameraToCharacterBack()
{
	FRotator CharacterRotation = GetActorRotation();
	CameraBoom->SetRelativeRotation(FRotator(-40.f, CharacterRotation.Yaw, 0.f));

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

// ---Getters And Setters---
FVector ABlasterCharacter::GetForwardVector() const
{
	return ForwardVector;
}

FVector ABlasterCharacter::GetRightVector() const
{
	return RightVector;
}
