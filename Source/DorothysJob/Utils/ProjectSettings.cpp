/************************************************************************
 * @description: Enables fast and easy access to Project Settings in Blueprints and C++.
 * @author: Jaime Paramo
 * @date: 19/03/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#include "ProjectSettings.h"

FString ProjectSettings::GetProjectVersion()
{
    FString sProjectVersion;

    GConfig->GetString(
        TEXT("/Script/EngineSettings.GeneralProjectSettings"),
        TEXT("ProjectVersion"),
        sProjectVersion,
        GGameIni
    );

    return sProjectVersion;
}