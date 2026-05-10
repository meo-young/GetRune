#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RuneInfo.h"
#include "SkillInfo.generated.h"

class USkillInfo;
class UNiagaraSystem;
class UAnimMontage;
class UGameplayEffect;

USTRUCT()
struct FSkillTierData
{
	GENERATED_BODY()
	
public:
	/** 1단계 스킬입니다. */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "1단계"))
	TObjectPtr<USkillInfo> SkillTier_1;
	
	/** 2단계 스킬입니다. */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "2단계"))
	TObjectPtr<USkillInfo> SkillTier_2;
	
	/** 3단계 스킬입니다. */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "3단계"))
	TObjectPtr<USkillInfo> SkillTier_3;
	
};

USTRUCT()
struct FRuneSkillEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (DisplayName = "룬 타입"))
	ERuneType RuneType;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "스킬 종류"))
	FSkillTierData SkillTierData;
};

UENUM()
enum class EStatusEffectType : uint8
{
	None = 0 UMETA(DisplayName = "없음")
};

UENUM(Blueprintable)
enum class ESkillType : uint8
{
	Instant UMETA(DisplayName = "즉발형"),
	Aim     UMETA(DisplayName = "조준형")
};

UENUM(Blueprintable)
enum class ESkillRank : uint8
{
	D UMETA(DisplayName = "D등급"),
	C UMETA(DisplayName = "C등급"),
	B UMETA(DisplayName = "B등급"),
	A UMETA(DisplayName = "A등급")
};

USTRUCT()
struct FNiagaraEffectInfo
{
	GENERATED_BODY()
	
public:
	/** 이펙트에 포함되는 사운드입니다. */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "효과음"))
	TObjectPtr<USoundCue> Sound;
	
	/** 이펙트입니다. */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "이펙트"))
	TObjectPtr<UNiagaraSystem> Effect;
	
};

UCLASS()
class GETRUNE_API USkillInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	/** 스킬 이름입니다. */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "이름"))
	FName SkillName;

	/** 스킬의 타입입니다. */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "타입"))
	ESkillType SkillType;
	
	/** 스킬 지속시간입니다. */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "지속시간"))
	float Duration;
	
	/** 에너지 개수당 데미지입니다. */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "에너지 개수당 데미지"))
	float EnergyPerDamage;

	/** 데미지를 적용하는 GameplayEffect입니다. */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "데미지 이펙트"))
	TSubclassOf<UGameplayEffect> DamageEffect;
	
	/** 스킬의 이동 속도입니다. */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "스피드"))
	float SkillSpeed;
	
	/** 스킬이 부여하는 상태이상입니다. */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "상태이상"))
	EStatusEffectType StatusEffectType;
	
	/** 스킬 등급입니다. */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "등급"))
	ESkillRank SkillRank;
	
	/** 공격 몽타주입니다. */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "공격 몽타주"))
	TObjectPtr<UAnimMontage> AttackMontage;

	/** 스킬 발동 시 출력하는 이펙트입니다. */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "발사 이펙트"))
	FNiagaraEffectInfo MuzzleEffect;
	
	/** 스킬 발동 간 출력하는 이펙트입니다. */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "투사체 이펙트"))
	FNiagaraEffectInfo ProjectileEffect;
	
	/** 스킬 피격 시 출력하는 이펙트입니다. */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "피격 이펙트"))
	FNiagaraEffectInfo HitEffect;
	
};