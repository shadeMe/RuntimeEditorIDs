#include "RuntimeEditorIDsInternals.h"

IDebugLog					gLog("RuntimeEditorIDs.log");

PluginHandle				g_pluginHandle = kPluginHandle_Invalid;

extern "C" {

bool OBSEPlugin_Query(const OBSEInterface * obse, PluginInfo * info)
{
	info->infoVersion = PluginInfo::kInfoVersion;
	info->name = "RuntimeEditorIDs";
	info->version = 1;

	if(!obse->isEditor)
	{
		if(obse->obseVersion < OBSE_VERSION_INTEGER)
		{
			_ERROR("OBSE version too old (got %08X expected at least %08X)", obse->obseVersion, OBSE_VERSION_INTEGER);
			return false;
		}

		if(obse->oblivionVersion != OBLIVION_VERSION)
		{
			_ERROR("incorrect Oblivion version (got %08X need %08X)", obse->oblivionVersion, OBLIVION_VERSION);
			return false;
		}
	}
	else
	{
		return false;		// as they say, don't fudge with the editor
	}

	return true;
}

bool OBSEPlugin_Load(const OBSEInterface * obse)
{
	g_pluginHandle = obse->GetPluginHandle();
	_MESSAGE("RuntimeEditorIDs Initializing...");
	gLog.Indent();

	for (int i = 0; i < VTBLTableSize; i++)
	{
		UInt32 PatchAddress = g_VTBLTable[i].Address + kTESForm_SetEditorID_VTBLOffset;
		SafeWrite32(PatchAddress, (UInt32)TESForm_HandleEditorID);

		_MESSAGE("Patched '%s' VTBL offset 0x%08X", g_VTBLTable[i].Class, PatchAddress);
	}

	gLog.Outdent();
	_MESSAGE("RuntimeEditorIDs Initialized!\n\n");
	return true;
}

};
