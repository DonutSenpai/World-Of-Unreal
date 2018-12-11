#pragma once

#define AddOnScreenDebugMsg(x) if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString::Printf(TEXT(x) ) )
