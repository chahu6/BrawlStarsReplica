#include "Components/GetActorScreenPointComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"

UGetActorScreenPointComponent::UGetActorScreenPointComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UGetActorScreenPointComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UGetActorScreenPointComponent::GetActorScreenPoint(FVector2D& GetActorScreenPoint, FVector2D& GetActorScreenMousePoint)
{
	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		FVector Location = MyOwner->GetActorLocation();
		FVector2D ScreenLocation;
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (PlayerController)
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
