#pragma once

#include "CoreMinimal.h"
#include "Blaster/Weapon/Weapon.h"
#include "Engine/Engine.h"
#include "GameFramework/Character.h"
#include "BlasterCharacter.generated.h"

UCLASS()
class BLASTER_API ABlasterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABlasterCharacter();
	virtual void PostInitializeComponents() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
	void RotateCameraToCharacterBack();

	void SetOverlappingWeapon(AWeapon* Weapon);

protected:
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void RotateCharacterToMouseCursor();
	void EquipButtonPressed();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta=(AllowPrivateAccess="true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta=(AllowPrivateAccess="true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	class UWidgetComponent* OverheadWidget;

	UPROPERTY(ReplicatedUsing=OnRep_OverlappingWeapon)
	class AWeapon* OverlappingWeapon;

	UPROPERTY(VisibleAnywhere)
	class UCombatComponent* CombatComponent;

	class Logger* Logger;

	void CameraSetup();
	void TestTextWidgetSetup();
	void CombatComponentSetup();

	UFUNCTION()
	FRotator DefineRotationByMousePosition(FVector2D MousePosition, APlayerController* PlayerController) const;

	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);

	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed();
};
