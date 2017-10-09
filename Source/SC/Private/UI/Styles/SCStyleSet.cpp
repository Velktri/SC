#include "SCStyleSet.h"
#include "SlateGameResources.h"
#include "SC.h"

TSharedPtr<FSlateStyleSet> FSCStyleSet::SCStyleInstance = NULL;

void FSCStyleSet::Initialize()
{
	if (!SCStyleInstance.IsValid())
	{
		SCStyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*SCStyleInstance);
	}
}

void FSCStyleSet::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*SCStyleInstance);
	ensure(SCStyleInstance.IsUnique());
	SCStyleInstance.Reset();
}

//const class ISlateStyle& FSCStyleSet::Get()
//{
//	return *SCStyleInstance;
//}

FName FSCStyleSet::GetStyleSetName()
{
	static FName StyleSetName(TEXT("SCStyles"));
	return StyleSetName;
}

TSharedRef<class FSlateStyleSet> FSCStyleSet::Create()
{
	TSharedRef<FSlateStyleSet> StyleRef = FSlateGameResources::New(FSCStyleSet::GetStyleSetName(), "/Game/UI/Styles", "/Game/UI/Styles");
	return StyleRef;
}
