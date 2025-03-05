#include "Skills/Active/Area/DOSA_Area.h"

#include "Kismet/KismetSystemLibrary.h"

UDOSA_Area::UDOSA_Area()
{
}

void UDOSA_Area::UseSkill_Implementation()
{
	Super::UseSkill_Implementation();

	TArray<FHitResult> hitResults;
	bool result{};
	//트레이싱 타입에 맞춰서 다른 트레이싱을 한다
	switch (traceType)
	{
	case ESKILL_TRACE_TYPE::SPHERE: {
		result = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(),
			startLocation,
			endLocation,
			radius,
			objectType,
			false,
			ignoreActors,
			EDrawDebugTrace::ForDuration,
			hitResults,
			true);
		break;
	}
	case ESKILL_TRACE_TYPE::BOX: {
		result = UKismetSystemLibrary::BoxTraceMultiForObjects(GetWorld(),
			startLocation,
			endLocation,
			halfSize,
			orientation,
			objectType,
			false,
			ignoreActors,
			EDrawDebugTrace::ForDuration,
			hitResults,
			true);
		break;
	}
	case ESKILL_TRACE_TYPE::CAPSULE: {
		result = UKismetSystemLibrary::CapsuleTraceMultiForObjects(GetWorld(),
			startLocation,
			endLocation,
			radius,
			halfHeight,
			objectType,
			false,
			ignoreActors,
			EDrawDebugTrace::ForDuration,
			hitResults,
			true);
		break;
	}
	}

	if (result) {
		for (int32 i = 0; i < hitResults.Num(); ++i)
			for (auto ignoreTag : ignoreTags)
				if (hitResults[i].GetActor()->Tags.Contains(ignoreTag)) {
					hitResults.RemoveAt(i);
					--i;
				}

		AreaCheckResults(hitResults);
	}
}

void UDOSA_Area::AreaCheckResults_Implementation(const TArray<FHitResult>& _HitResults)
{
	DFLOG_N(Display);
}
