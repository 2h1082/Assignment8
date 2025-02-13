// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpartaCharacter.generated.h"

class USpringArmComponent; //스프링 암 관련 클래스 헤더
class UCameraComponent; //카메라 관련 클래스 전방 선언
class UWidgetComponent;
//Enhanced Input에서 액션 값 받을 때 사용할 구조체
struct FInputActionValue;

UCLASS()
class SPARTAPROJECT_API ASpartaCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASpartaCharacter();

	//스프링암
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;
	//카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* OverheadWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> WBP_HPClass;

	//현재 체력 가져오는 함수
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealth() const;
	//최대 체력 가져오는 함수
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Health")
	void AddHealth(float Amount);
	//속도 감소

	UFUNCTION(BlueprintCallable, Category = "Debuff")
	void SlowSpeed(float SlowAmount);
	UFUNCTION(BlueprintCallable, Category = "Debuff")
	void EndSlow();

	//방향 반전
	UFUNCTION(BlueprintCallable, Category = "Debuff")
	void ReverseControl();
	UFUNCTION(BlueprintCallable, Category = "Debuff")
	void EndReverseControl();

	UFUNCTION(BlueprintPure, Category = "Debuff")
	bool bIsSlow() const;
	UFUNCTION(BlueprintPure, Category = "Debuff")
	bool bIsInversion() const;

	//버프 타이머 핸들러
	FTimerHandle SlowTimerHandle;
	FTimerHandle InversionTimerHandle;

protected:

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Movement")
	float NormalSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeedMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeed;
	//슬로우 상태 표시
	bool bHasSlowDebuff;
	//방향 반전 상태 표시
	bool bHasControlInversionDebuff;

	//최대 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;
	//현재 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health;
	//사망 처리 함수
	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual void OnDeath();
	void UpdateOverheadHP();

	virtual void BeginPlay() override;
	//데미지 처리 함수
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void StartJump(const FInputActionValue& value);
	UFUNCTION()
	void StopJump(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void StartSprint(const FInputActionValue& value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& value);
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
