// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSwitchWeapon, FWeaponSlot, WeaponSlot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAmmoChange, EWeaponType, TypeAmmo, int32, _Count);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponAmmoEmpty, EWeaponType, DWeaponType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFireInventory, int32, CurrentWeaponIndexInventory);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReloadInInventory, int32, CurrentWeaponIndexInventory);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventoryWeaponPickUpSuccess, int32, IndexOfNewWeapon, FWeaponSlot, WeaponSlot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventoryAmmoPickUpSuccess, int32, IndexOfNewAmmo, FAmmoSlot, AmmoSlot);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TASKCASE_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	//������� �� ����� ������
	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		FOnSwitchWeapon OnSwitchWeapon;

	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		FOnAmmoChange OnAmmoChange;

	//�������, ����� ����������� �������
	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		FOnWeaponAmmoEmpty OnWeaponAmmoEmpty;

	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		FOnFireInventory OnFireInventory;

	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		FOnReloadInInventory OnReloadInInventory;

	//������� ����� ��������� ���������� ������ � ���������
	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		FOnInventoryWeaponPickUpSuccess OnInventoryWeaponPickUpSuccess;

	//������� ����� ��������� ���������� ������ ���� ������ � ���������
	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		FOnInventoryAmmoPickUpSuccess OnInventoryAmmoPickUpSuccess;



protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
		TArray<FWeaponSlot> WeaponSlots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
		TArray<FAmmoSlot> AmmoSlots;

	UPROPERTY(BlueprintReadOnly)
		int32 CurrentIndex = 0;


	UFUNCTION(BlueprintCallable)
		void SwitchWeaponToIndex(int32 ChangeToIndex, FAdditionalWeaponInfo OldInfo);


	UFUNCTION(BlueprintCallable)
		int32 GetWeaponIndexSlotByName(FName IdWeaponName);

	void SetAdditionalInfoWeapon(int32 IndexWeapon, FAdditionalWeaponInfo NewInfo);

	bool CharEquipedPistol = false;

	bool CheckAmmoForWeapon(EWeaponType _TypeWeapon);
	void FOnFireInInventory();
	void ReloadInInventory();

	UFUNCTION(BlueprintCallable)
		int32 ReturnLastingAmmo(EWeaponType _TypeWeapon);

	//������� �������� �������� ��� �� �������
	UFUNCTION(BlueprintCallable)
		void AmmoSlotChangeValue(EWeaponType _WeaponType, int32 CountChangeAmmo);

	int32 MaxWeaponInInventory = 4;

	//Interface Pickup Actors
	UFUNCTION(BlueprintCallable, Category = "Item pickup Interface")
		bool CheckCanTakeAmmo(EWeaponType AmmoType);
	UFUNCTION(BlueprintCallable, Category = "Item pickup Interface")
		bool CheckCanTakeWeapon(int32& FreeSlotIndex, FName _NewWeaponName);
	UFUNCTION(BlueprintCallable, Category = "Item pickup Interface")
		void SaveItemToInventory();



	UFUNCTION(BlueprintCallable, Category = "Item pickup Interface")
		void SwitchWeaponToInventory(FWeaponSlot NewWeapon, int32 IndexOfSlot);

	//���������� ������ � ���������
	UFUNCTION(BlueprintCallable, Category = "Item pickup Interface")
		bool TryGetWeaponToInventory(FWeaponSlot NewWeapon);

	int32 GetAmmoIndexByType(EWeaponType _WeaponType);


	UFUNCTION(BlueprintCallable, Category = "Inventory")
		FName GetWeaponNameByIndex(int32 IndexSlot);


	UFUNCTION(BlueprintCallable, Category = "Inventory")
		int32 GetCurrentIndex();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void InitInventory(TArray<FWeaponSlot> WeaponSlotsToInit, TArray<FAmmoSlot> AmmoSlotToInit);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void SwapWeapon(int32 IndexToChange);



		
};
