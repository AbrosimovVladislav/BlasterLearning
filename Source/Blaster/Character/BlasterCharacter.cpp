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

void ABlasterCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// DOREPLIFETIME(ABlasterCharacter, OverlappingWeapon);
	DOREPLIFETIME_CONDITION(ABlasterCharacter, OverlappingWeapon, COND_OwnerOnly);
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


// ---PickUp Widget---
void ABlasterCharacter::OnRep_OverlappingWeapon(AWeapon* LastWeapon)
{
	GEngine->AddOnScreenDebugMessage(
		-1,
		5.f,
		FColor::Purple,
		FString::Printf(TEXT("Role: %s, OverlappingWeapon: %s"), TEXT("OnRep_OverlappingWeapon:"),
		                OverlappingWeapon ? *OverlappingWeapon->GetName() : TEXT("None"))
	);

	// Will be called just on client
	//If OverlappingWeapon==null, then we gonna skip this if
	if (OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickUpWidget(true);
	}
	//And we gonna manipulate with LastWeapon before OverlappingWeapon became null
	if (LastWeapon)
	{
		LastWeapon->ShowPickUpWidget(false);
	}
}

void ABlasterCharacter::SetOverlappingWeapon(AWeapon* Weapon)
{
	// Will be called just on server, because this will called from AWeapon::OnSphereOverlap which is only called from server

	//If there was a weapon before, switch the widget
	if (OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickUpWidget(false);
	}

	//Set new weapon even if it is null
	OverlappingWeapon = Weapon;
	if (IsLocallyControlled())
	{
		//If new weapon not null, show widget on it
		if (OverlappingWeapon)
		{
			OverlappingWeapon->ShowPickUpWidget(true);
		}
	}
}
