#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BlasterCharacter.generated.h"

UCLASS()
class BLASTER_API ABlasterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABlasterCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
	void RotateCameraToCharacterBack();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_UpdateCharacterRotation(const FRotator& NewRotation, FVector NewForwardVector, FVector NewRightVector);

	UPROPERTY(ReplicatedUsing = OnRep_CharacterRotation, VisibleAnywhere, BlueprintReadOnly, Category = "Rotation")
	FRotator CharacterRotation;

	//Getters Setters
	FRotator GetCharacterRotation() const;

protected:
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void RotateCharacterToMouseCursor();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta=(AllowPrivateAccess="true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta=(AllowPrivateAccess="true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	class UWidgetComponent* OverheadWidget;

	void CameraSetup();
	void TestTextWidgetSetup();

	UFUNCTION()
	void OnRep_CharacterRotation();

	UFUNCTION()
	FRotator DefineRotationByMousePosition(FVector2D MousePosition, APlayerController* PlayerController) const;
};
