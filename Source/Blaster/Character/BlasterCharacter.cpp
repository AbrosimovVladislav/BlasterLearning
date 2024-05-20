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

void ABlasterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABlasterCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABlasterCharacter::MoveRight);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
}

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

void ABlasterCharacter::MoveForward(float Value)
{
	if (Controller != nullptr && Value != 0.f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ABlasterCharacter::MoveRight(float Value)
{
	if (Controller != nullptr && Value != 0.f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void ABlasterCharacter::RotateCharacterToMouseCursor()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		FVector2D MousePosition;
		if (PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y))
		{
			FVector WorldDirection;
			FVector WorldLocation;
			PlayerController->DeprojectScreenPositionToWorld(
				MousePosition.X, MousePosition.Y, WorldLocation, WorldDirection);
			FVector TargetLocation = WorldLocation + WorldDirection * 1000;
			FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation);
			SetActorRotation(FRotator(0.0f, NewRotation.Yaw, 0.0f));
		}
	}
}

void ABlasterCharacter::CameraSetup()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 900.f;
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
