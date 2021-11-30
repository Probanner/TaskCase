// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "DefaultWeapon.h"
#include "Types.h"
#include "CharHealthComponent.h"
#include "InventoryComponent.h"
#include "TestCaseInstance.h"
#include "GameFramework/Character.h"
#include "TaskCaseCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UMotionControllerComponent;
class UAnimMontage;
class USoundBase;

UCLASS(config=Game)
class ATaskCaseCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;


	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health, meta = (AllowPrivateAccess = "true"))
	UCharHealthComponent* CharHealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	UInventoryComponent* CharInventoryComponent;


public:
	ATaskCaseCharacter();

	DECLARE_DELEGATE_OneParam(DELEGATE_OnenumberPressed, int32);
	DECLARE_DELEGATE_OneParam(DELEGATE_TwonumberPressed, int32);
	DECLARE_DELEGATE_OneParam(DELEGATE_ThreenumberPressed, int32);



protected:
	virtual void BeginPlay();

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;


protected:
	
	/** Fires a projectile. */
	void OnFireStart();
	UFUNCTION()
	void FireMontage(bool _bHaveAnim);

	/** Fires a projectile. */
	void OnFireEnd();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface


public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	UFUNCTION()
	void WeaponInit(FWeaponSlot _NewWeaponSlot);

	//Текущее оружие
	ADefaultWeapon* CurrentWeapon = nullptr;
	UFUNCTION(BlueprintCallable)
	ADefaultWeapon* GetCurrentWeapon();
	UFUNCTION()
	void WeaponReloadStart();
	UFUNCTION()
	void WeaponReloadEnd();
	bool IsReloading = false;

	void TryReloadWeapon();

	UFUNCTION(BlueprintCallable)
	int GetAmmoTypeIndex();

	bool bIsAlive = true;

	void WeaponChangeCharacterForDelegate(int32 ChangeToIndex);
	void StopAllAminChar();
	
};

