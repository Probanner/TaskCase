// Copyright Epic Games, Inc. All Rights Reserved.

#include "TaskCaseCharacter.h"
#include "TaskCaseProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId


DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// ATaskCaseCharacter

ATaskCaseCharacter::ATaskCaseCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));


	CharHealthComponent = CreateDefaultSubobject<UCharHealthComponent>(TEXT("CharHealthComponent"));
	CharInventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("CharInventoryComponent"));
	if (CharInventoryComponent)
	{
		CharInventoryComponent->OnSwitchWeapon.AddDynamic(this, &ATaskCaseCharacter::WeaponInit);
	}

}

void ATaskCaseCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	//WeaponInit();
}

//////////////////////////////////////////////////////////////////////////
// Input

void ATaskCaseCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ATaskCaseCharacter::OnFireStart);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ATaskCaseCharacter::OnFireEnd);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &ATaskCaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATaskCaseCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ATaskCaseCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ATaskCaseCharacter::LookUpAtRate);

	//Перезарядка
	PlayerInputComponent->BindAction(TEXT("Reload"), EInputEvent::IE_Released, this, &ATaskCaseCharacter::TryReloadWeapon);

	PlayerInputComponent->BindAction<DELEGATE_OnenumberPressed>("SwitchWeapon0", IE_Pressed, this, &ATaskCaseCharacter::WeaponChangeCharacterForDelegate, int32(0));

	PlayerInputComponent->BindAction<DELEGATE_TwonumberPressed>("SwitchWeapon1", IE_Pressed, this, &ATaskCaseCharacter::WeaponChangeCharacterForDelegate, int32(1));

	PlayerInputComponent->BindAction<DELEGATE_ThreenumberPressed>("SwitchWeapon2", IE_Pressed, this, &ATaskCaseCharacter::WeaponChangeCharacterForDelegate, int32(2));

}





void ATaskCaseCharacter::WeaponInit(FWeaponSlot _NewWeaponSlot)
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
	}

	UTestCaseInstance* myGI = Cast<UTestCaseInstance>(GetGameInstance());
	FWeaponInfo myWeaponInfo;

	if (myGI)
	{
		if (myGI->GetWeaponInfoByName(_NewWeaponSlot.ItemName, myWeaponInfo))
		{
			
			if (myWeaponInfo.WeaponClass)
			{
				FVector SpawnLocation = FVector(0);
				FRotator SpawnRotation = FRotator(0);
				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = GetInstigator();
				//Спауним оружие
				if (myWeaponInfo.WeaponEquipAnimMontage)
				{
					Mesh1P->GetAnimInstance()->Montage_Play(myWeaponInfo.WeaponEquipAnimMontage, myWeaponInfo.WeaponEquipAnimMontage->GetMaxCurrentTime() / myWeaponInfo.AnimEquipTime);
				}

				ADefaultWeapon* myWeapon = Cast<ADefaultWeapon>(GetWorld()->SpawnActor(myWeaponInfo.WeaponClass, &SpawnLocation, &SpawnRotation, SpawnParams));
				if (myWeapon)
				{
					CurrentWeapon = myWeapon;
					CurrentWeapon = myWeapon;
					myWeapon->WeaponSetting = myWeaponInfo;
					myWeapon->CurrentWeaponIdName = _NewWeaponSlot.ItemName;
					myWeapon->AdditionalWeaponInfo = _NewWeaponSlot.AdditionalInfo;
					myWeapon->ReloadTime = myWeaponInfo.ReloadTime;
					myWeapon->OnWeaponReloadStart.AddDynamic(this, &ATaskCaseCharacter::WeaponReloadStart);
					myWeapon->OnWeaponReloadEnd.AddDynamic(this, &ATaskCaseCharacter::WeaponReloadEnd);
					myWeapon->OnFire.AddDynamic(this, &ATaskCaseCharacter::FireMontage);
					myWeapon->SkeletalMeshWeapon->SetVisibility(false);
				
					FAttachmentTransformRules Rule(EAttachmentRule::SnapToTarget, false);

					myWeapon->AttachToComponent(Mesh1P, FAttachmentTransformRules::FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false), myWeaponInfo.InitSocketName);
				}

			}
		}
	}
	
}

void ATaskCaseCharacter::WeaponReloadStart()
{
	IsReloading = true;

	if (GetCurrentWeapon()->WeaponSetting.AnimCharReload)
	{
		Mesh1P->GetAnimInstance()->Montage_Play(GetCurrentWeapon()->WeaponSetting.AnimCharReload,
			GetCurrentWeapon()->WeaponSetting.AnimCharReload->GetMaxCurrentTime() / GetCurrentWeapon()->WeaponSetting.ReloadTime);
	}
}

void ATaskCaseCharacter::TryReloadWeapon()
{
	if (GetAmmoTypeIndex() != -1 && bIsAlive)
	{
		if (CurrentWeapon && CharInventoryComponent->AmmoSlots[GetAmmoTypeIndex()].Count != 0)
		{
			if (CurrentWeapon->GetWeaponRound() < CurrentWeapon->WeaponSetting.MaxRound && !CurrentWeapon->WeaponReloading)
			{
				CurrentWeapon->InitReload();
			}
		}
	}
}

// Проверка на соответсвие патрона и текущего оружия
int ATaskCaseCharacter::GetAmmoTypeIndex()
{
	for (int i = 0; i < CharInventoryComponent->AmmoSlots.Num(); i++)
	{
		if (CharInventoryComponent->AmmoSlots[i].WeaponType == GetCurrentWeapon()->WeaponSetting.WeaponType)
		{
			return i;
		}
	}
	return -1;
}

//Смена оружия
void ATaskCaseCharacter::WeaponChangeCharacterForDelegate(int32 ChangeToIndex)
{
	UTestCaseInstance* myGI = Cast<UTestCaseInstance>(GetWorld()->GetGameInstance());

	if (myGI)
	{
		if (ChangeToIndex + 1 <= CharInventoryComponent->WeaponSlots.Num()
			&& ChangeToIndex != CharInventoryComponent->GetCurrentIndex()
			&& !CharInventoryComponent->WeaponSlots[ChangeToIndex].ItemName.IsNone()
			&& myGI->GetWeaponInfoByName(CharInventoryComponent->WeaponSlots[ChangeToIndex].ItemName, GetCurrentWeapon()->WeaponSetting))
		{
			StopAllAminChar();
			GetCurrentWeapon()->OnWeaponReloadEnd.Broadcast();

			CharInventoryComponent->SwitchWeaponToIndex(ChangeToIndex, GetCurrentWeapon()->AdditionalWeaponInfo);
		}
	}
}
//Остановка анимации при переключении оружия
void ATaskCaseCharacter::StopAllAminChar()
{
	if (Mesh1P->GetAnimInstance())
	{
		Mesh1P->GetAnimInstance()->StopAllMontages(0.15f);
	}

	IsReloading = false;

	if (GetCurrentWeapon())
	{
		if (GetCurrentWeapon()->SkeletalMeshWeapon && GetCurrentWeapon()->SkeletalMeshWeapon->GetAnimInstance())
		{
			GetCurrentWeapon()->SkeletalMeshWeapon->GetAnimInstance()->StopAllMontages(0.15f);
			GetCurrentWeapon()->SkeletalMeshWeapon->GetAnimInstance()->StopSlotAnimation(0.15f);
		}

		if (GetCurrentWeapon()->ReloadSound)
		{
			if (GetCurrentWeapon()->ReloadSound->IsPlaying())
			{
				GetCurrentWeapon()->ReloadSound->Deactivate();
			}

		}
	}
}

void ATaskCaseCharacter::WeaponReloadEnd()
{
	IsReloading = false;
}

ADefaultWeapon* ATaskCaseCharacter::GetCurrentWeapon()
{
	return CurrentWeapon;
}

void ATaskCaseCharacter::OnFireStart()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->SetWeaponStateFire(true);
	}
}

void ATaskCaseCharacter::FireMontage(bool _bHaveAnim)
{
	if (_bHaveAnim)
	{
		if (GetCurrentWeapon()->WeaponSetting.HipAnimCharFire)
		{
			Mesh1P->GetAnimInstance()->Montage_Play(GetCurrentWeapon()->WeaponSetting.HipAnimCharFire, 1);
		}
	}
	CharInventoryComponent->WeaponSlots[CharInventoryComponent->GetCurrentIndex()].AdditionalInfo.Round = GetCurrentWeapon()->GetWeaponRound();
}



void ATaskCaseCharacter::OnFireEnd()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->SetWeaponStateFire(false);
	}
}

void ATaskCaseCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ATaskCaseCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ATaskCaseCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ATaskCaseCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}
