// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types.h"
#include "Engine/GameInstance.h"
#include "TestCaseInstance.generated.h"

/**
 * 
 */
UCLASS()
class TASKCASE_API UTestCaseInstance : public UGameInstance
{
	GENERATED_BODY()
public:
		//table
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponSetting")
		UDataTable* WeaponInfoTable = nullptr;

	UFUNCTION(BlueprintCallable)
		bool GetWeaponInfoByName(FName NameWeapon, FWeaponInfo& OutInfo);


};
