// Fill out your copyright notice in the Description page of Project Settings.


#include "ABaseItem.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
// Sets default values
AABaseItem::AABaseItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//루트 컴포넌트 생성 및 설정
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	//충돌 컴포넌트 생성 및 설정
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	//겹침만 감지하는 프로파일 설정
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	//루트 컴포넌트로 설정
	Collision->SetupAttachment(Scene);

	//스태틱 메시 컴포넌트 생성 및 설정
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Collision);
	//메시가 불필요하게 충돌을 막지 않도록 하기 위해, 별도로 NoCollision 등으로 설정 가능

	//Overlap 이벤트 바인딩
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AABaseItem::OnItemOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &AABaseItem::OnItemEndOverlap);

}

void AABaseItem::OnItemOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//OtherActor가 플레이어인지 확인
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		//아이템 사용 로직 호출
		ActivateItem(OtherActor);
	}
}

void AABaseItem::OnItemEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AABaseItem::ActivateItem(AActor* Activator)
{
	UParticleSystemComponent* Particle = nullptr;

	if (PickupParticle)
	{
		Particle = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			PickupParticle,
			GetActorLocation(),
			GetActorRotation(),
			true
		);
	}
	if (PickupSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			PickupSound,
			GetActorLocation()
		);
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
}

FName AABaseItem::GetItemType() const
{
	return FName();
}

void AABaseItem::DestroyItem()
{
	//객체 제거
	Destroy();
}

void AABaseItem::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//타이머 제거
	GetWorldTimerManager().ClearTimer(DestroyParticleTimerHandle);
}


