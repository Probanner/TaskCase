// Fill out your copyright notice in the Description page of Project Settings.


#include "CharHealthComponent.h"

void UCharHealthComponent::ChangeCurrentHealth(float ChangeValue)
{
	float CurrentDamage = ChangeValue * DamageCoef;

	if (Health < 100.0f && ChangeValue>0)
	{
		Super::ChangeCurrentHealth(ChangeValue);
		//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("Health < 100.0f && ChangeValue>0"));
	}
	else if (CharShield > 0.0f)
	{
		ChangeShieldValue(CurrentDamage);
		//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("(CharShield > 0.0f)"));
		if (CharShield < 0.0f)
		{
			//FX
		}

	}//MedKit
	else //Damage to Health
	{
		Super::ChangeCurrentHealth(ChangeValue);
		//GEngine->AddOnScreenDebugMessage(-1,10,FColor::Green, TEXT("Super::ChangeCurrentHealth(ChangeValue)"));
	}
	// for char

}

void UCharHealthComponent::ChangeShieldValue(float ChangeShieldValue)
{
	CharShield += ChangeShieldValue;
	if (CharShield > 100.0f)
	{
		CharShield = 100.0f;
	}
	else if (CharShield < 0)
	{
		CharShield = 0.0f;
	}

	OnShieldChange.Broadcast(CharShield, ChangeShieldValue);
}

float UCharHealthComponent::GetCurrentShield()
{
	return CharShield;
}

void UCharHealthComponent::SetCurrentShieldValue(float Value)
{
	CharShield = Value;
}
