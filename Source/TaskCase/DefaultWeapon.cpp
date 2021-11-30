// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultWeapon.h"

#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "InventoryComponent.h"

// Sets default values
ADefaultWeapon::ADefaultWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//Создаем компонент Сцены
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	//Rootcomponent наследуется от Actor.h
	RootComponent = SceneComponent;

	SkeletalMeshWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMeshWeapon->SetGenerateOverlapEvents(false);
	SkeletalMeshWeapon->SetCollisionProfileName(TEXT("NoCollision"));
	SkeletalMeshWeapon->SetupAttachment(RootComponent);

	ShootLocationArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ShootLocationArrow->SetupAttachment(SkeletalMeshWeapon);

}

// Called when the game starts or when spawned
void ADefaultWeapon::BeginPlay()
{
	Super::BeginPlay();
	ViewPortSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
	ViewPortCenter = FVector2D(ViewPortSize.X / 2, ViewPortSize.Y / 2);

}

// Called every frame
void ADefaultWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FireTick(DeltaTime);
	ReloadTick(DeltaTime);
}

void ADefaultWeapon::FireTick(float DeltaTime)
{
	if (GetWeaponRound() > 0)
	{
		if (WeaponFiring && !WeaponReloading)
		{
			if (FireTime < 0.f)
			{
				Fire();
			}
			else
			{
				FireTime -= DeltaTime;
			}
		}
	}
	else
	{
		if (!WeaponReloading && CanWeaponReload())
		{
			InitReload();
		}
	}
}

void ADefaultWeapon::ReloadTick(float DeltaTime)
{
	if (WeaponReloading)
	{
		if (ReloadTime <= 0.0f)
		{
			FinishReload();
		}
		else
		{
			ReloadTime -= DeltaTime;
		}
	}
}

int32 ADefaultWeapon::GetWeaponRound()
{
	return AdditionalWeaponInfo.Round;
}


bool ADefaultWeapon::CanWeaponReload()
{
	if (GetOwner())
	{

		UInventoryComponent* MyInv = Cast<UInventoryComponent>(GetOwner()->GetComponentByClass(UInventoryComponent::StaticClass()));
		if (MyInv)
		{
			if (!MyInv->CheckAmmoForWeapon(WeaponSetting.WeaponType))
			{
				return false;
			}
		}
	}

	return true;
}


void ADefaultWeapon::Fire()
{
	if (bIsEquiped)
	{
		if (WeaponSetting.ProjectileSetting.ProjectileClass)
		{
			FireTime = WeaponSetting.RateOfFire;
		

			if (ShootLocationArrow)
			{
			
				if (PlayerController)
				{
					PlayerController->DeprojectScreenPositionToWorld((ViewPortSize.X / 2)+5, (ViewPortSize.Y / 2), OutWorldLocation, OutWorldDirection);

					FCollisionQueryParams myTraceParam;
					FHitResult myResult1;
					myTraceParam.bTraceComplex = true;

					FVector Dir = ShootLocationArrow->GetComponentLocation() + OutWorldDirection;

					FRotator RightRotation = (Dir - ShootLocationArrow->GetComponentLocation()).Rotation();
					ShootLocationArrow->SetWorldRotation(RightRotation);


					//DrawDebugLine(GetWorld(), OutWorldLocation, OutWorldLocation+ OutWorldDirection*300, FColor::Red, false, 4, 0, 2);

					bool Result = GetWorld()->LineTraceSingleByChannel(myResult1, OutWorldLocation, OutWorldLocation + OutWorldDirection * 500, ECC_Visibility, myTraceParam);
				
					for (int i = 0; i < WeaponSetting.BulletPerShot; i++)
					{
						//Simple dispertion
						float DispertionY = FMath::RandRange(-0.03f,0.03f);
						float DispertionZ = FMath::RandRange(-0.03f, 0.03f);

						//В зависимости от расстояния от персонажа до преграды, разный тип стрельбы
						if (Result)
						{
							FVector CorrectionVector = FVector(0, i*DispertionY*200, i*DispertionZ*200);
							RightRotation = ((myResult1.ImpactPoint + CorrectionVector) - ShootLocationArrow->GetComponentLocation()).Rotation();

							FVector StartShootLocation = ShootLocationArrow->GetComponentLocation();
							FRotator StartShootRotation = RightRotation;

							FActorSpawnParameters ActorSpawnParams;
							ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

					
							ProjectileRespawn(WeaponSetting.ProjectileSetting.ProjectileClass,
								WeaponSetting.ProjectileSetting.ProjectileSpeed,
								StartShootLocation, StartShootRotation,
								WeaponSetting.ProjectileSetting.ProjectileLifeTime,
								WeaponSetting.ProjectileSetting);
						}
						else
						{
							Dir = Dir + FVector(0, i * DispertionY, i * DispertionZ);

							RightRotation = (Dir - ShootLocationArrow->GetComponentLocation()).Rotation();

							ShootLocationArrow->SetWorldRotation(RightRotation);

							FVector StartShootLocation = ShootLocationArrow->GetComponentLocation();
							FRotator StartShootRotation = ShootLocationArrow->GetComponentRotation();

							FActorSpawnParameters ActorSpawnParams;
							ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

							//Функция, которая переопределена в BluePrint'е оружия
							ProjectileRespawn(WeaponSetting.ProjectileSetting.ProjectileClass,
								WeaponSetting.ProjectileSetting.ProjectileSpeed,
								StartShootLocation, StartShootRotation,
								WeaponSetting.ProjectileSetting.ProjectileLifeTime,
								WeaponSetting.ProjectileSetting);
						}

					}

					UGameplayStatics::SpawnSoundAtLocation(GetWorld(), WeaponSetting.SoundFireWeapon, ShootLocationArrow->GetComponentLocation());	
				}

			}
			AdditionalWeaponInfo.Round--;
			if (GetOwner())
			{

				UInventoryComponent* MyInv = Cast<UInventoryComponent>(GetOwner()->GetComponentByClass(UInventoryComponent::StaticClass()));
				if (MyInv)
				{
					MyInv->FOnFireInInventory();
				}
			}
			if (WeaponSetting.HipAnimCharFire)
			{
				OnFire.Broadcast(true);
			}
			else
			{
				OnFire.Broadcast(false);

			}
		}
	}
}

void ADefaultWeapon::InitReload()
{
	if (bIsEquiped)
	{
		WeaponReloading = true;
		ReloadTime = WeaponSetting.ReloadTime;
		ReloadSound = nullptr;
		if (WeaponSetting.SoundReload)
		{
			ReloadSound = UGameplayStatics::SpawnSound2D(GetWorld(), WeaponSetting.SoundReload, 1.0f, 1.0f, 0.0f, nullptr, false, false);
		}

		OnWeaponReloadStart.Broadcast();
	}

}
//Получаем доступное количество патронов для перезарядки
int32 ADefaultWeapon::GetAvailableAmmo()
{
	int8 ResultAmmo = AdditionalWeaponInfo.Round;
	if (GetOwner())
	{
		UInventoryComponent* MyInv = Cast<UInventoryComponent>(GetOwner()->GetComponentByClass(UInventoryComponent::StaticClass()));
		if (!MyInv)
		{
			return 0;
		}
		else
		{
			int8 AmmoIndex = 0;
			if (MyInv->GetAmmoIndexByType(WeaponSetting.WeaponType) != -1)
			{
				AmmoIndex = MyInv->GetAmmoIndexByType(WeaponSetting.WeaponType);

				if (ResultAmmo + MyInv->AmmoSlots[AmmoIndex].Count >= WeaponSetting.MaxRound)
				{
					MyInv->AmmoSlots[AmmoIndex].Count = MyInv->AmmoSlots[AmmoIndex].Count
						- (WeaponSetting.MaxRound - ResultAmmo);

					//Патроны в обойме класса Оружия
					ResultAmmo += WeaponSetting.MaxRound - ResultAmmo;

					//Патроны в обойме класса инвентаря
					MyInv->WeaponSlots[MyInv->GetCurrentIndex()].AdditionalInfo.Round = ResultAmmo;
				}
				else
				{
					//Патроны в обойме класса Оружия
					ResultAmmo += MyInv->AmmoSlots[AmmoIndex].Count;

					//Патроны в обойме класса инвентаря
					MyInv->WeaponSlots[MyInv->GetCurrentIndex()].AdditionalInfo.Round = ResultAmmo;
					MyInv->AmmoSlots[AmmoIndex].Count = 0;
				}


			}
			else
			{
				return 0;
			}
		}
	}

	return ResultAmmo;
}



FWeaponInfo ADefaultWeapon::GetWeaponSetting()
{
	return WeaponSetting;
}

void ADefaultWeapon::FinishReload()
{
	WeaponReloading = false;

	AdditionalWeaponInfo.Round = GetAvailableAmmo();

	OnWeaponReloadEnd.Broadcast();

	if (GetOwner())
	{
		UInventoryComponent* MyInv = Cast<UInventoryComponent>(GetOwner()->GetComponentByClass(UInventoryComponent::StaticClass()));
		if (MyInv)
		{
			MyInv->ReloadInInventory();
		}
	}
}



void ADefaultWeapon::SetWeaponStateFire(bool blsFire)
{
	WeaponFiring = blsFire;
	FireTime = 0.0f;
}

