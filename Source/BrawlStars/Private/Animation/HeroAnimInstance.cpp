#include "Animation/HeroAnimInstance.h"
#include "Hero/HeroBase.h"
#include "GameFramework/CharacterMovementComponent.h"

void UHeroAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<AHeroBase>(TryGetPawnOwner());
}

void UHeroAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (Character == nullptr)
	{
		Character = Cast<AHeroBase>(TryGetPawnOwner());
	}

	if (Character == nullptr) return;

	Speed = Character->GetVelocity().Size2D();

	bIsAir = Character->GetCharacterMovement()->IsFalling();

	bIsAccelerating = Character->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.0f;

	bIsCrouched = Character->GetCharacterMovement()->IsCrouching();
}
