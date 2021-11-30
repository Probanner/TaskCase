// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "DefaultWeapon.h"
#include "Engine/DataTable.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Types.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	RifleType UMETA(DisplayName = "Rifle"),
	ShotgunType UMETA(DisplayName = "Shotgun"),
};

USTRUCT(BlueprintType)
struct FProjectileInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSetting")
	TSubclassOf<class ATaskCaseProjectile> ProjectileClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSetting")
		float ProjectileDamage = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSetting")
		float ProjectileLifeTime = 8.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSetting")
		float ProjectileSpeed = 2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSetting")
		USoundBase* HitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSetting")
		 UMaterialInterface* HitDecal;
};



USTRUCT(BlueprintType)
struct FWeaponInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class")
		TSubclassOf<class ADefaultWeapon> WeaponClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		float RateOfFire = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		float ReloadTime = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		int32 MaxRound = 30;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		int32 BulletPerShot = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Name")
		FName WeaponName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponType")
		EWeaponType WeaponType = EWeaponType::RifleType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		USoundBase* SoundFireWeapon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		USoundBase* SoundReload = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		FProjectileInfo ProjectileSetting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitEffect")
		UDecalComponent* DecalOnHit = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* HipAnimCharFire = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* WeaponEquipAnimMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		float AnimEquipTime = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* AnimCharReload = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InitSocket")
		FName InitSocketName;

	//Inventory
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		UTexture2D* WeaponIcon = nullptr;
};


USTRUCT(BlueprintType)
struct FAdditionalWeaponInfo
{
	GENERATED_BODY()

		//Количество патронов в обойме
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponStats")
		int32 Round = 10;
};


// Индекс, имя, и количество патронов в магазине у конкретного Weapon
USTRUCT(BlueprintType)
struct FWeaponSlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponSlot")
		FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponType")
		EWeaponType WeaponType = EWeaponType::RifleType;

	//Структура, там пока только количество патронов в обойме
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponSlot")
		FAdditionalWeaponInfo AdditionalInfo;
};

USTRUCT(BlueprintType)
struct FAmmoSlot
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AmmoSlot")
		EWeaponType WeaponType = EWeaponType::RifleType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AmmoSlot")
		int32 Count = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AmmoSlot")
		int32 MaxCount = 100;
};


UCLASS()
class TASKCASE_API UTypes : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
};
