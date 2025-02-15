// Fill out your copyright notice in the Description page of Project Settings.


#include "MineItem.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AMineItem::AMineItem()
{
	ExplosionDelay = 5.0f;
	ExplosionRadius = 300.0f;
	ExplosionDamage = 30.0f;
	ItemType = "Mine";
	bHasExploded = false;

	ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
	ExplosionCollision->InitSphereRadius(ExplosionRadius);
	ExplosionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ExplosionCollision->SetupAttachment(Scene);
}

void AMineItem::ActivateItem(AActor* Activator)
{
	if (bHasExploded) return;

	Super::ActivateItem(Activator);
	GetWorld()->GetTimerManager().SetTimer(ExplosionTimerHandle, this, &AMineItem::Explosion, ExplosionDelay,false);

	bHasExploded = true;
}

void AMineItem::Explosion()
{
	UParticleSystemComponent* Particle = nullptr;

	if (ExplosionParticle)
	{
		Particle = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld()->PersistentLevel,
			ExplosionParticle,
			GetActorLocation(),
			GetActorRotation(),
			false
		);
	}

	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			ExplosionSound,
			GetActorLocation()
		);
	}

	TArray<AActor*> OverlappingActors;
	ExplosionCollision->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (Actor && Actor->ActorHasTag("Player"))
		{
			//데미지 발생시켜 Actor->TakeDamage() 호출하도록 만듬
			UGameplayStatics::ApplyDamage(
				Actor,
				ExplosionDamage,
				nullptr,
				this,
				UDamageType::StaticClass()
			);
		}
	}

	if (Particle)
	{
		GetWorld()->GetTimerManager().SetTimer(
			DestroyParticleTimerHandle,
			[Particle]()
			{
				// GameThread에서 DestroyComponent() 실행
				AsyncTask(ENamedThreads::GameThread, [Particle]()
					{
						if (Particle)
						{
							Particle->DestroyComponent();
						}
					});
			},
			2.0f,
			false
		);
	}

	DestroyItem();

}

//EndPlay() 에서 모든 타이머 해제
void AMineItem::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// ExplosionTimerHandle 해제
	if (ExplosionTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(ExplosionTimerHandle);
	}

	// DestroyParticleTimerHandle 해제
	if (DestroyParticleTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(DestroyParticleTimerHandle);
	}
}