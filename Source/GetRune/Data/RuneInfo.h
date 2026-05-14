#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RuneInfo.generated.h"

class AGRRuneBase;
class AGRGreenRune;
class AGRBlueRune;
class AGRRedRune;
class UNiagaraSystem;

UENUM(BlueprintType)
enum class ERuneType : uint8
{
	Red UMETA(DisplayName = "빨간색"),
	Green UMETA(DisplayName = "초록색"),
	Blue UMETA(DisplayName = "파란색")
};

USTRUCT(BlueprintType)
struct FRuneClassData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (DisplayName = "룬 클래스"))
	TSubclassOf<AGRRuneBase> RuneClass;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "공격 이펙트"))
	TObjectPtr<UNiagaraSystem> AttackEffect;
};

UCLASS()
class GETRUNE_API URuneInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (DisplayName = "룬 종류"))
	TMap<ERuneType, FRuneClassData> RuneClass;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "소환 최소 범위"))
	float InnerRadius = 400.f;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "소환 최대 범위"))
	float OuterRadius = 700.f;


};
