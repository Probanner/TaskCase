// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "TaskCaseCharacter.h"
#include "DefaultWeapon.h"
#include "TestCaseInstance.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	//делегат для кэректора


}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();


}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryComponent::SwitchWeaponToIndex(int32 ChangeToIndex, FAdditionalWeaponInfo OldInfo)
{
	if (WeaponSlots.IsValidIndex(ChangeToIndex))
	{

		OnSwitchWeapon.Broadcast(WeaponSlots[ChangeToIndex]);

		CurrentIndex = GetCurrentIndex();

	}
	else
	{
		OnSwitchWeapon.Broadcast(WeaponSlots[0]);
	}


}

//Находит индекс оружия по имени
int32 UInventoryComponent::GetWeaponIndexSlotByName(FName IdWeaponName)
{
	int32 FoundedId = 0;
	for (int i = 0; i < WeaponSlots.Num(); i++)
	{
		if (WeaponSlots[i].ItemName == IdWeaponName)
			FoundedId = i;

	}
	return FoundedId;
}

bool UInventoryComponent::CheckAmmoForWeapon(EWeaponType _TypeWeapon)
{
	for (int i = 0; i < AmmoSlots.Num(); i++)
	{
		if (AmmoSlots[i].WeaponType == _TypeWeapon && AmmoSlots[i].Count > 0)
		{
			return true;
		}
	}

	OnWeaponAmmoEmpty.Broadcast(_TypeWeapon);

	return false;
}

void UInventoryComponent::FOnFireInInventory()
{
	OnFireInventory.Broadcast(GetCurrentIndex());
}

void UInventoryComponent::ReloadInInventory()
{
	OnReloadInInventory.Broadcast(GetCurrentIndex());
}

int32 UInventoryComponent::ReturnLastingAmmo(EWeaponType _TypeWeapon)
{

	for (int i = 0; i < AmmoSlots.Num(); i++)
	{
		if (AmmoSlots[i].WeaponType == _TypeWeapon)
		{
			return AmmoSlots[i].Count;
		}
	}

	return 0;
}

bool UInventoryComponent::CheckCanTakeAmmo(EWeaponType AmmoType)
{	 
	for (int i = 0; i < AmmoSlots.Num(); i++)
	{
		for (int j = 0; j < AmmoSlots.Num(); j++)
		{
			if (AmmoSlots[j].Count < AmmoSlots[j].MaxCount && AmmoSlots[j].WeaponType == AmmoType)
			{
				return true;
			}
			else if (AmmoSlots[j].Count >= AmmoSlots[j].MaxCount && AmmoSlots[j].WeaponType == AmmoType)
			{
				return false;
			}
		
		}
		
		if (i== AmmoSlots.Num() - 1 && AmmoSlots[i].WeaponType != AmmoType)
		{
			FAmmoSlot NewSlot;
			NewSlot.Count = 0;
			NewSlot.MaxCount = 100;
			NewSlot.WeaponType = AmmoType;
			AmmoSlots.Add(NewSlot);
			OnInventoryAmmoPickUpSuccess.Broadcast(AmmoSlots.Num()-1, AmmoSlots[AmmoSlots.Num() - 1]);
			return true;
		}
	}
	return false;
}

//Проверка на свободный слот


void UInventoryComponent::SaveItemToInventory()
{

}

//Замена текущего оружия, оружием на полу
void UInventoryComponent::SwitchWeaponToInventory(FWeaponSlot NewWeapon, int32 IndexOfSlot)
{
	if (WeaponSlots.IsValidIndex(IndexOfSlot))
	{
		WeaponSlots[IndexOfSlot] = NewWeapon;
		OnSwitchWeapon.Broadcast(NewWeapon);
	}
}



FName UInventoryComponent::GetWeaponNameByIndex(int32 IndexSlot)
{
	FName NameToReturn;
	if (WeaponSlots.IsValidIndex(IndexSlot))
	{
		NameToReturn = WeaponSlots[IndexSlot].ItemName;
	}

	return NameToReturn;
}


int32 UInventoryComponent::GetCurrentIndex()
{

	if (GetOwner())
	{

		ATaskCaseCharacter* MyChar = Cast<ATaskCaseCharacter>(GetOwner());

		ADefaultWeapon* MyWeapon = Cast<ADefaultWeapon>(MyChar->GetCurrentWeapon());
		if (MyWeapon)
		{
			for (int i = 0; i < WeaponSlots.Num(); i++)
			{
				if (MyWeapon->WeaponSetting.WeaponName == WeaponSlots[i].ItemName)
				{
					CurrentIndex = i;
					return i;
				}
			}
		}
		else
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("UInventoryComponent::GetCurrentIndex ||| (MyWeapon): FALSE"));
			}
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("UInventoryComponent::GetCurrentIndex ||| GetOwner: FALSE"));
		}
	}

	return 0;
}

void UInventoryComponent::InitInventory(TArray<FWeaponSlot> WeaponSlotsToInit, TArray<FAmmoSlot> AmmoSlotToInit)
{
	WeaponSlots = WeaponSlotsToInit;
	AmmoSlots = AmmoSlotToInit;
}

void UInventoryComponent::SwapWeapon(int32 IndexToChange)
{
	if (WeaponSlots.IsValidIndex(IndexToChange))
	{
		if (!WeaponSlots[0].ItemName.IsNone())
		{
			OnSwitchWeapon.Broadcast(WeaponSlots[IndexToChange]);

		}
	}
	CurrentIndex = IndexToChange;
	FOnFireInInventory();
}

//добавление оружия к инвентарю
bool UInventoryComponent::TryGetWeaponToInventory(FWeaponSlot NewWeapon)
{
	int32 FreeIndexSlot = -1;
	if (CheckCanTakeWeapon(FreeIndexSlot, NewWeapon.ItemName))
	{
		if (WeaponSlots.IsValidIndex(FreeIndexSlot))
		{
			WeaponSlots[FreeIndexSlot] = NewWeapon;
			OnInventoryWeaponPickUpSuccess.Broadcast(FreeIndexSlot, WeaponSlots[FreeIndexSlot]);
			return true;
		}

	}
	return false;
}

bool UInventoryComponent::CheckCanTakeWeapon(int32& FreeSlotIndex, FName _NewWeaponName)
{

	UTestCaseInstance* myGI = Cast<UTestCaseInstance>(GetWorld()->GetGameInstance());
	FWeaponInfo TempWeaponInfo;
	if (myGI)
	{
		for (int i = 0; i < WeaponSlots.Num(); i++)
		{

			//Проверка на пустой слот БЕЗ ИМЕНИ или с ИМЕНЕМ оружия, которого НЕТ в таблице

			if (WeaponSlots[i].ItemName.IsNone()
				//Если в WeaponSlot[i] записана билиберда
				|| myGI->GetWeaponInfoByName(WeaponSlots[i].ItemName, TempWeaponInfo) == false
				//Поиск нового оружия в таблице
				|| myGI->GetWeaponInfoByName(_NewWeaponName, TempWeaponInfo) == false
				&& WeaponSlots.Num() < MaxWeaponInInventory)
			{
				FreeSlotIndex = i;

				return true;
			}
			else if (WeaponSlots.Num() < MaxWeaponInInventory)
			{
				FWeaponSlot NewWeaponSlot;
				WeaponSlots.Add(NewWeaponSlot);
				FreeSlotIndex = WeaponSlots.Num() - 1;

				return true;
			}
		}
	}


	return false;
}



int32 UInventoryComponent::GetAmmoIndexByType(EWeaponType _WeaponType)
{
	for (int i = 0; i < AmmoSlots.Num(); i++)
	{
		if (AmmoSlots[i].WeaponType == _WeaponType)
		{
			return i;
		}
	}
	return -1;
}



void UInventoryComponent::AmmoSlotChangeValue(EWeaponType _WeaponType, int32 CountChangeAmmo)
{
	for (int i = 0; i < AmmoSlots.Num(); i++)
	{
		if (AmmoSlots[i].WeaponType == _WeaponType)
		{
			AmmoSlots[i].Count += CountChangeAmmo;
			if (AmmoSlots[i].Count > AmmoSlots[i].MaxCount)
			{
				AmmoSlots[i].Count = AmmoSlots[i].MaxCount;
			}
			//broadcast for widget
			OnAmmoChange.Broadcast(AmmoSlots[i].WeaponType, AmmoSlots[i].Count);
		}
	}

}

