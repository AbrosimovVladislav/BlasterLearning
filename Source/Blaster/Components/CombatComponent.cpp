#include "CombatComponent.h"

#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/Weapon/Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Net/UnrealNetwork.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// ---Setups---
void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCombatComponent, EquippedWeapon);
	DOREPLIFETIME(UCombatComponent, bAiming);
}

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

// ---Aiming And Shooting---
void UCombatComponent::SetAiming(bool bIsAiming)
{
	//client prediction changing of bAiming
	bAiming = bIsAiming;
	//send request to change bAiming on server, bAiming is replicated, so it will send new value to clients
	ServerSetAiming(bIsAiming);
}

void UCombatComponent::ServerSetAiming_Implementation(bool bIsAiming)
{
	bAiming = bIsAiming;
}
