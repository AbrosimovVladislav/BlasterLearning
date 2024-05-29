#include "CombatComponent.h"

#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/Weapon/Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/SkeletalMeshSocket.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// ---Setups---


// ---Begin And Tick---
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// ---Weapon equipment---
void UCombatComponent::EquipWeapon(AWeapon* WeaponToEquip)
{
	if (Character == nullptr || WeaponToEquip == nullptr) return;

	EquippedWeapon = WeaponToEquip;
	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
	USkeletalMeshComponent* CharacterSkeletalMesh = Character->GetMesh();
	const USkeletalMeshSocket* CharacterRightHandSocket = CharacterSkeletalMesh->GetSocketByName(
		FName("RightHandSocket"));
	if (CharacterRightHandSocket)
	{
		CharacterRightHandSocket->AttachActor(EquippedWeapon, CharacterSkeletalMesh);
	}
	EquippedWeapon->SetOwner(Character);
}
