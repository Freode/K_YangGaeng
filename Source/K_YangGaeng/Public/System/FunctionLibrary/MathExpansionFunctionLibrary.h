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
    static FORCEINLINE FTransform ChangeComponentTransformWorldToLocal(class UPrimitiveComponent* &InComponent, const FTransform& InTransform)
    {
        // return InTransform * UKismetMathLibrary::InvertTransform(InComponentTransform);
        return InTransform * UKismetMathLibrary::InvertTransform(InComponent->GetComponentTransform());
    }

    // Change component's transform from local space to world space
    UFUNCTION(BlueprintCallable, Category = "K_YG|Math")
    static FORCEINLINE FTransform ChangeComponentTransformLocalToWorld(class UPrimitiveComponent*& InComponent, const FTransform& InTransform)
    {
        return InTransform * InComponent->GetComponentTransform();
    }

    // Operate Transform A - Transform B
    UFUNCTION(BlueprintCallable, Category = "K_YG|Math")
    static FORCEINLINE FTransform TransformMinusOperation(const FTransform& A, const FTransform& B)
    {
        FVector Location = A.GetLocation() - B.GetLocation();
        FRotator Rotation = FRotator(A.GetRotation().Rotator().Pitch - B.GetRotation().Rotator().Pitch, A.GetRotation().Rotator().Yaw - B.GetRotation().Rotator().Yaw, A.GetRotation().Rotator().Roll - B.GetRotation().Rotator().Roll);
        FVector Scale3D = A.GetScale3D() - B.GetScale3D();
        return UKismetMathLibrary::MakeTransform(Location, Rotation, Scale3D);
    }

    // Operate Transform A + Transform B
    UFUNCTION(BlueprintCallable, Category = "K_YG|Math")
    static FORCEINLINE FTransform TransformAddOperation(const FTransform& A, const FTransform& B)
    {
        FVector Location = A.GetLocation() + B.GetLocation();
        FRotator Rotation = FRotator(A.GetRotation().Rotator().Pitch + B.GetRotation().Rotator().Pitch, A.GetRotation().Rotator().Yaw + B.GetRotation().Rotator().Yaw, A.GetRotation().Rotator().Roll + B.GetRotation().Rotator().Roll);
        FVector Scale3D = A.GetScale3D() + B.GetScale3D();
        return UKismetMathLibrary::MakeTransform(Location, Rotation, Scale3D);
    }
};
