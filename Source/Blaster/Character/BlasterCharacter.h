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
	void ChangeEquipped();

	UFUNCTION(BlueprintCallable)
	bool GetIsEquipped() const;
	
	UFUNCTION(BlueprintCallable)
	void RotateCameraToCharacterBack();

protected:
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void RotateCharacterToMouseCursor();

	UPROPERTY(Replicated)
	bool IsEquipped;

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetIsEquipped(bool NewValue);

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
	FRotator DefineRotationByMousePosition(FVector2D MousePosition, APlayerController* PlayerController) const;
};
