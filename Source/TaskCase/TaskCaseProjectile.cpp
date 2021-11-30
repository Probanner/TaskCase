// Copyright Epic Games, Inc. All Rights Reserved.

#include "TaskCaseProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"

ATaskCaseProjectile::ATaskCaseProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ATaskCaseProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	//ProjectileMovement->InitialSpeed = 10000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	//InitialLifeSpan = 30.0f;
}

void ATaskCaseProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	UGameplayStatics::ApplyDamage(OtherActor, ProjectioleInfo.ProjectileDamage, GetInstigatorController(), this, NULL);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(),HitSound, Hit.ImpactPoint);

	if (ProjectioleInfo.HitDecal)
	{
		UDecalComponent* myDecal = UGameplayStatics::SpawnDecalAttached(ProjectioleInfo.HitDecal, FVector(CollisionComp->GetScaledSphereRadius()), OtherComp,
			NAME_None, Hit.ImpactPoint, Hit.Normal.Rotation(), EAttachLocation::KeepWorldPosition, 5.0f);

		myDecal->SetFadeScreenSize(0.001f);
	}

	Destroy();
	
}

void ATaskCaseProjectile::InitProjectile(FProjectileInfo _Info)
{
	ProjectioleInfo = _Info;
}
