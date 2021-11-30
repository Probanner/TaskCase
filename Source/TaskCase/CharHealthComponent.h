// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HealthComponent.h"
#include "CharHealthComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnShieldChange, float, ShieldValue, float, Damage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShieldBroke);

/**
 * 
 */
UCLASS()
class TASKCASE_API UCharHealthComponent : public UHealthComponent
{
	GENERATED_BODY()
	
	public:
	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Shield")
	FOnShieldChange OnShieldChange;

	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Shield")
	FOnShieldBroke OnShieldBroke;


	protected:
	float CharShield = 100.0f;

	public:

	void ChangeCurrentHealth(float ChangeValue) override;

	UFUNCTION(BlueprintCallable)
	void ChangeShieldValue(float ChangeShieldValue);

	UFUNCTION(BlueprintCallable)
	float GetCurrentShield();

	UFUNCTION(BlueprintCallable)
	void SetCurrentShieldValue(float Value);

};
