// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "TaskCaseProjectile.h"
#include "GameFramework/Actor.h"
#include "Types.h"
#include "DefaultWeapon.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponReloadStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponReloadEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFire, bool, bHaveAnim);


UCLASS()
class TASKCASE_API ADefaultWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADefaultWeapon();

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
		USceneComponent* SceneComponent = nullptr;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
		USkeletalMeshComponent* SkeletalMeshWeapon = nullptr;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
		UArrowComponent* ShootLocationArrow = nullptr;

	//UPROPERTY(EditDefaultsOnly, Category = Projectile)
		//TSubclassOf<class ATaskCaseProjectile> ProjectileClass = nullptr;

	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		FOnWeaponReloadStart OnWeaponReloadStart;

	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		FOnWeaponReloadEnd OnWeaponReloadEnd;

	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		FOnFire OnFire;

	UPROPERTY()
		FWeaponInfo WeaponSetting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponInfo")
		FAdditionalWeaponInfo AdditionalWeaponInfo;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Функции для Tick'а
	void FireTick(float DeltaTime);
	void ReloadTick(float DeltaTime);
	void WeaponInit();

	float FireTime = 0.5f;

	void Fire();

	
	//Viewport Center
	FVector2D ViewPortSize;
	FVector2D ViewPortCenter;

	FVector OutWorldLocation = FVector(0);
	FVector OutWorldDirection = FVector(0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AmmoInMag = 30;

	UFUNCTION(BlueprintCallable)
	void SetWeaponStateFire(bool blsFire);

	const APlayerController* const PlayerController = Cast<const APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	
	

	FName CurrentWeaponIdName;

	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = "ReloadTime")
		float ReloadTime = 0.0f;

	UFUNCTION(BlueprintCallable)
	int32 GetWeaponRound();

	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = "FireLogic")
		bool WeaponFiring = false;
	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = "FireLogic")
		bool WeaponReloading = false;

	void InitReload();
	bool CanWeaponReload();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Equiped")
		bool bIsEquiped = false;

	UAudioComponent* ReloadSound = nullptr;

	void FinishReload();
	int32 GetAvailableAmmo();

	UFUNCTION(BlueprintCallable)
		FWeaponInfo GetWeaponSetting();


	UFUNCTION(BlueprintImplementableEvent)
		void ProjectileRespawn(TSubclassOf<ATaskCaseProjectile> _ProjectileToRespawn,
			float _initialSpeed, FVector _SpawnLocation,
			FRotator _SpawnRotation,
			float _SpanLifeTime,
			FProjectileInfo _ProjectileInfo);

};
