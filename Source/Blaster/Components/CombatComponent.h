#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BLASTER_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatComponent();
	friend class ABlasterCharacter;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	void EquipWeapon(class AWeapon* WeaponToEquip);
	
protected:
	virtual void BeginPlay() override;

private:
	// Will be settled inside ABlasterCharacter::PostInitializeComponents()
	ABlasterCharacter* Character;
	
	AWeapon* EquippedWeapon;

	
};
