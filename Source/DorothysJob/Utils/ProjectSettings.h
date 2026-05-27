/************************************************************************
 * @description: Enables fast and easy access to Project Settings in Blueprints and C++.
 * @author: Jaime Paramo
 * @date: 19/03/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"

class DOROTHYSJOB_API ProjectSettings
{
public:
    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Project Version"), Category = "Project Settings")
    static FString GetProjectVersion(); 
};