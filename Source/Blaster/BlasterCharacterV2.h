#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BlasterCharacterV2.generated.h"

UCLASS()
class BLASTER_API ABlasterCharacterV2 : public ACharacter
{
	GENERATED_BODY()

public:
	ABlasterCharacterV2();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
		virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
};
