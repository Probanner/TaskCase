// Fill out your copyright notice in the Description page of Project Settings.


#include "TestCaseInstance.h"

bool UTestCaseInstance::GetWeaponInfoByName(FName NameWeapon, FWeaponInfo& OutInfo)
{
	bool bIsFind = false;
	FWeaponInfo* WeaponInfoRow;
	if (WeaponInfoTable)
	{
		WeaponInfoRow = WeaponInfoTable->FindRow<FWeaponInfo>(NameWeapon, "", false);
		if (WeaponInfoRow)
		{
			bIsFind = true;
			OutInfo = *WeaponInfoRow;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UHomeWork22Instance::GetWeaponInfoByName - WeaponTable -NULL"));
	}
	return bIsFind;

}


