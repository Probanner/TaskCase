// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChange, float, Health, float, Damage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDead);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TASKCASE_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Health")
		FOnHealthChange OnHeathChange;
	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Health")
		FOnDead OnDead;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	float Health = 100.0;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float DamageCoef = 1.0f;

	UFUNCTION(BlueprintCallable, Category = "Health")
		float GetCurrentHealth();

	UFUNCTION(BlueprintCallable, Category = "Health")
		void SetCurrentHeath(float _health);

	UFUNCTION(BlueprintCallable, Category = "Health")
		virtual	void ChangeCurrentHealth(float ChangeValue);

	UFUNCTION(BlueprintNativeEvent)
		void DeadEvent();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsAlive = true;
		
};
