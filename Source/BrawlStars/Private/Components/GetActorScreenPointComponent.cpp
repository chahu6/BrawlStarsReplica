#include "Components/GetActorScreenPointComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"

UGetActorScreenPointComponent::UGetActorScreenPointComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UGetActorScreenPointComponent::GetActorScreenPoint(FVector2D& GetActorScreenPoint, FVector2D& GetActorScreenMousePoint)
{
	if (AActor* MyOwner = GetOwner())
	{
		const FVector Location = MyOwner->GetActorLocation();
		FVector2D ScreenLocation;
		if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0))
		{
			PlayerController->ProjectWorldLocationToScreen(Location, ScreenLocation);
			PlayerController->SetMouseLocation(FMath::TruncToInt(ScreenLocation.X), FMath::TruncToInt(ScreenLocation.Y));
		}

		const FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
		if (MousePosition.X > 0.0f && MousePosition.Y > 0.0f)
		{
			GetActorScreenPoint = ScreenLocation;
			GetActorScreenMousePoint = MousePosition;
			return true;
		}
		else
		{
			GetActorScreenPoint = FVector2D::Zero();
			GetActorScreenMousePoint = FVector2D::Zero();
		}
	}

	return false;
}
