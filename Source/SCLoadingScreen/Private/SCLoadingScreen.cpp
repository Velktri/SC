#include "SCLoadingScreen/Public/SCLoadingScreen.h"

void FSCLoadingScreenModule::StartupModule()
{
	UE_LOG(LogTemp, Warning, TEXT("Loading Screen module has started!"));
}

void FSCLoadingScreenModule::ShutdownModule()
{
	UE_LOG(LogTemp, Warning, TEXT("Loading Screen module has shut down"));
}

IMPLEMENT_GAME_MODULE(FSCLoadingScreenModule, SCLoadingScreen)