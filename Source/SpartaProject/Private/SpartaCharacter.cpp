// Fill out your copyright notice in the Description page of Project Settings.


#include "SpartaCharacter.h"
#include "SpartaPlayerController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"
#include "SpartaGameState.h"
#include "Components/ProgressBar.h"


// Sets default values
ASpartaCharacter::ASpartaCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//create SpringArm
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	//attach to root component
	SpringArmComp->SetupAttachment(RootComponent);
	//set default value for distance(character~camera)
	SpringArmComp->TargetArmLength = 300.0f;
	//spring arm rotate with controller move
	SpringArmComp->bUsePawnControlRotation = true;

	//create camera
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	//attach to spring arm
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	//off setting about PawnControlRotation
	CameraComp->bUsePawnControlRotation = false;

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(GetMesh());
	OverheadWidget->SetWidgetSpace(EWidgetSpace::Screen);

	NormalSpeed = 600.0f;
	SprintSpeedMultiplier = 1.5f;
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;

	//초기 체력 설정
	MaxHealth = 100.0f;
	Health = MaxHealth;

	bHasSlowDebuff = false;
	bHasControlInversionDebuff = false;
}

float ASpartaCharacter::GetHealth() const
{
	return Health;
}

float ASpartaCharacter::GetMaxHealth() const
{
	return MaxHealth;
}

void ASpartaCharacter::AddHealth(float Amount)
{
	//체력 회복
	Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);
	UpdateOverheadHP();
}

void ASpartaCharacter::SlowSpeed(float SlowAmount)
{
	NormalSpeed = 600.0f*SlowAmount/100;
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;
	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	bHasSlowDebuff = true;

	GetWorldTimerManager().ClearTimer(SlowTimerHandle);
	float RemainTime=GetWorldTimerManager().GetTimerRemaining(SlowTimerHandle);
	GetWorldTimerManager().SetTimer(SlowTimerHandle, this, &ASpartaCharacter::EndSlow, 5.0f+RemainTime, false);
}

void ASpartaCharacter::EndSlow()
{
	NormalSpeed = 600.0f;
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;
	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	bHasSlowDebuff = false;
}

void ASpartaCharacter::ReverseControl()
{
	bHasControlInversionDebuff = true;
	GetWorldTimerManager().ClearTimer(InversionTimerHandle);
	float RemainTime = GetWorldTimerManager().GetTimerRemaining(InversionTimerHandle);
	GetWorldTimerManager().SetTimer(InversionTimerHandle, this, &ASpartaCharacter::EndReverseControl, 5.0f+RemainTime, false);
}

void ASpartaCharacter::EndReverseControl()
{
	bHasControlInversionDebuff = false;
}

bool ASpartaCharacter::bIsSlow() const
{
	return bHasSlowDebuff;
}

bool ASpartaCharacter::bIsInversion() const
{
	return bHasControlInversionDebuff;
}

void ASpartaCharacter::OnDeath()
{
	ASpartaGameState* SpartaGameState = GetWorld() ? GetWorld()->GetGameState<ASpartaGameState>() : nullptr;
	if (SpartaGameState)
	{
		SpartaGameState->OnGameOver();
	}
}

void ASpartaCharacter::UpdateOverheadHP() //블루 프린트로 구현
{
	if (!OverheadWidget) return;

	UUserWidget* OverheadWidgetInstance = OverheadWidget->GetUserWidgetObject();
	if (!OverheadWidgetInstance) return;

	if (UProgressBar* HPBar = Cast<UProgressBar>(OverheadWidgetInstance->GetWidgetFromName(TEXT("HealthBar"))))
	{
		HPBar->SetPercent(FMath::Clamp(Health / MaxHealth,0.0f,1.0f));
	}
}

void ASpartaCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (WBP_HPClass && OverheadWidget)
	{
		OverheadWidget->SetWidgetClass(WBP_HPClass);
		OverheadWidget->InitWidget();
	}
	UpdateOverheadHP();
}

float ASpartaCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//기본 데미지 처리 로직
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	//체력 데미지만큼 감소
	Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);
	UpdateOverheadHP();

	//체력 0이 되면 사망 처리
	if (Health <= 0.0f)
	{
		OnDeath();
	}
	//실제 적용된 데미지 반환
	return ActualDamage;
}

// Called to bind functionality to input
void ASpartaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ASpartaPlayerController* PlayerController = Cast<ASpartaPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&ASpartaCharacter::Move);
			}
			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Triggered,
					this,
					&ASpartaCharacter::StartJump);
				
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Completed,
					this,
					&ASpartaCharacter::StopJump);
			}
			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&ASpartaCharacter::Look);
			}
			if (PlayerController->SprintAction)
			{
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Triggered,
					this,
					&ASpartaCharacter::StartSprint);

				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Completed,
					this,
					&ASpartaCharacter::StopSprint);
			}
		}
	}
}

void ASpartaCharacter::Move(const FInputActionValue& value)
{
	if (!Controller) return;

	FVector2D MoveInput = value.Get<FVector2D>();

	//방향 반전 디버프 적용되면 입력값 반전
	if (bHasControlInversionDebuff)
	{
		MoveInput = FVector2D(-MoveInput.X, -MoveInput.Y);
	}
	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
	}
	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}
}

void ASpartaCharacter::StartJump(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		Jump();
	}
}

void ASpartaCharacter::StopJump(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		StopJumping();
	}
}

void ASpartaCharacter::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();

	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

void ASpartaCharacter::StartSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void ASpartaCharacter::StopSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}

//EndPlay() 에서 모든 타이머 해제
void ASpartaCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// SlowTimerHandle 해제
	if (SlowTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(SlowTimerHandle);
	}

	// InversionTimerHandle 해제
	if (InversionTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(InversionTimerHandle);
	}
}