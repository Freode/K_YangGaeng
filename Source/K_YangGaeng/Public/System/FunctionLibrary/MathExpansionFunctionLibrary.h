// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "MathExpansionFunctionLibrary.generated.h"

/**
 *
 */
UCLASS()
class K_YANGGAENG_API UMathExpansionFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

    public:

    // Chagne component's transform from world space to local space
    UFUNCTION(BlueprintCallable, Category = "K_YG|Math")
    static FORCEINLINE FTransform ChangeComponentTransformWorldToLocal(const class UPrimitiveComponent* InComponent, const FTransform& InTransform)
    {
        return InTransform * UKismetMathLibrary::InvertTransform(InComponent->GetComponentTransform());
    }

    // Change component's transform from local space to world space
    UFUNCTION(BlueprintCallable, Category = "K_YG|Math")
    static FORCEINLINE FTransform ChangeComponentTransformLocalToWorld(const class UPrimitiveComponent* InComponent, const FTransform& InTransform)
    {
        return InTransform * InComponent->GetComponentTransform();
    }

    // Operate Transform A - Transform B
    UFUNCTION(BlueprintCallable, Category = "K_YG|Math")
    static FORCEINLINE FTransform TransformMinusOperation(const FTransform& A, const FTransform& B)
    {
        FVector Location = A.GetLocation() - B.GetLocation();
        FRotator Rotation = FRotator(A.GetRotation().X - B.GetRotation().X, A.GetRotation().Y - B.GetRotation().Y, A.GetRotation().Z - B.GetRotation().Z);
        FVector Scale3D = A.GetScale3D() - B.GetScale3D();
        return UKismetMathLibrary::MakeTransform(Location, Rotation, Scale3D);
    }
};
