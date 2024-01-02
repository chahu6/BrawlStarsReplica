#pragma once

#include "CoreMinimal.h"
#include "DataType.generated.h"

UENUM(BlueprintType)
enum class EGameLevelName : uint8
{
	None			UMETA(DisplayName = "None"),
	EGN_Heist		UMETA(DisplayName = "L_Heist"),
	EGN_GemGrab		UMETA(DisplayName = "L_GemGrab")
};

UENUM(BlueprintType)
enum class EBattleType : uint8
{
	None			UMETA(DisplayName = "None"),
};

UENUM(BlueprintType)
enum class ESessionType : uint8
{
	None			UMETA(DisplayName = "None"),
	EST_Game		UMETA(DisplayName = "Game"),
	EST_Room		UMETA(DisplayName = "Room"),
};

UENUM(BlueprintType)
enum class ESessionExtraSetting : uint8
{
	None				UMETA(DisplayName = "None"),
	ES_SessionType		UMETA(DisplayName = "SessionType"),
	ES_IsMatching		UMETA(DisplayName = "IsMatching"),
	ES_GameLevelName	UMETA(DisplayName = "GameLevelName")
};

UENUM(BlueprintType)
enum class ETeamType : uint8
{
	None				UMETA(DisplayName = "None"),
	ET_TeamA			UMETA(DisplayName = "TeamA"),
	ET_TeamB			UMETA(DisplayName = "TeamB"),
};