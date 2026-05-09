#include "GRDamageExecution.h"
#include "GetRune/AbilitySystem/Attributes/GRCombatSet.h"
#include "GetRune/AbilitySystem/Attributes/GRHealthSet.h"

/**
 * FDamageStatics
 *
 * Execute_Implementation이 호출될 때마다 캡처 정의를 새로 생성하지 않도록
 * 싱글톤으로 관리하는 헬퍼 구조체입니다.
 *
 * Source: 데미지를 가하는 쪽 (적), bSnapshot=true: GE 생성 시점의 값을 스냅샷으로 사용합니다.
 */
struct FDamageStatics
{
	FGameplayEffectAttributeCaptureDefinition BaseDamageDef;

	FDamageStatics()
	{
		BaseDamageDef = FGameplayEffectAttributeCaptureDefinition(
			UGRCombatSet::GetBaseDamageAttribute(),
			EGameplayEffectAttributeCaptureSource::Source,
			true
		);
	}
};

static FDamageStatics& DamageStatics()
{
	static FDamageStatics Statics;
	return Statics;
}

UGRDamageExecution::UGRDamageExecution()
{
	// Execute_Implementation에서 읽어올 어트리뷰트를 미리 등록합니다.
	// 등록하지 않으면 AttemptCalculateCapturedAttributeMagnitude 호출 시 0이 반환됩니다.
	RelevantAttributesToCapture.Add(DamageStatics().BaseDamageDef);
}

void UGRDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	float BaseDamage = 0.0f;
	{
		FAggregatorEvaluateParameters EvaluateParameters;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BaseDamageDef, EvaluateParameters, BaseDamage);
	}

	const float DamageDone = FMath::Max(BaseDamage, 0.0f);
	if (DamageDone > 0.0f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UGRHealthSet::GetDamageAttribute(), EGameplayModOp::Additive, DamageDone));
	}
}
