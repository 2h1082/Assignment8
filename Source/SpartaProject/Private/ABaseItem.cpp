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

	//��Ʈ ������Ʈ ���� �� ����
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	//�浹 ������Ʈ ���� �� ����
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	//��ħ�� �����ϴ� �������� ����
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	//��Ʈ ������Ʈ�� ����
	Collision->SetupAttachment(Scene);

	//����ƽ �޽� ������Ʈ ���� �� ����
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Collision);
	//�޽ð� ���ʿ��ϰ� �浹�� ���� �ʵ��� �ϱ� ����, ������ NoCollision ������ ���� ����

	//Overlap �̺�Ʈ ���ε�
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AABaseItem::OnItemOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &AABaseItem::OnItemEndOverlap);

}

void AABaseItem::OnItemOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//OtherActor�� �÷��̾����� Ȯ��
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		//������ ��� ���� ȣ��
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
				// GameThread���� DestroyComponent() ����
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
	//��ü ����
	Destroy();
}

void AABaseItem::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//Ÿ�̸� ����
	GetWorldTimerManager().ClearTimer(DestroyParticleTimerHandle);
}


