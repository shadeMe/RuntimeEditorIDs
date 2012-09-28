#include "RuntimeEditorIDsInternals.h"
#include "Commands.h"
#include "VersionInfo.h"

IDebugLog					gLog("RuntimeEditorIDs.log");

PluginHandle				g_pluginHandle = kPluginHandle_Invalid;
OBSEStringVarInterface*		g_strVarInfc = NULL;
OBSEMessagingInterface*		g_msgIntfc = NULL;

CSEIntelliSenseInterface*	g_CSEISIntfc	= NULL;
CSEConsoleInterface*		g_CSEConsoleIntfc	= NULL;

void REIDInteropHandler(OBSEMessagingInterface::Message* Msg)
{
	switch (Msg->type)
	{
	case 'CSEI':
		{
			CSEInterface* Interface = (CSEInterface*)Msg->data;

			g_CSEConsoleIntfc = (CSEConsoleInterface*)Interface->InitializeInterface(CSEInterface::kCSEInterface_Console);
			g_CSEISIntfc = (CSEIntelliSenseInterface*)Interface->InitializeInterface(CSEInterface::kCSEInterface_IntelliSense);

			_MESSAGE("Received interface from CSE");

			g_CSEConsoleIntfc->PrintToConsole("REID", "Registering command URLs ...");
			g_CSEISIntfc->RegisterCommandURL("GetRuntimeEditorID", "http://cs.elderscrolls.com/constwiki/index.php/GetRuntimeEditorID");
		}

		break;
	case 'EDID':
		{
			REIDInteropData* Data = (REIDInteropData*)Msg->data;

			if (Data)
			{
				const char* EditorID = g_editorIDManager.LookupByFormID(Data->FormID);

				if (EditorID)
				{
					Data->HasEditorID = true;
					strcpy_s(Data->EditorIDBuffer, Data->kBufferSize, EditorID);
				}
				else
					Data->HasEditorID = false;
			}
		}

		break;
	}
}

void OBSEMessageHandler(OBSEMessagingInterface::Message* Msg)
{
	switch (Msg->type)
	{
	case OBSEMessagingInterface::kMessage_PostLoad:
		g_msgIntfc->RegisterListener(g_pluginHandle, NULL, REIDInteropHandler);
		g_msgIntfc->RegisterListener(g_pluginHandle, "CSE", REIDInteropHandler);

		break;
	case OBSEMessagingInterface::kMessage_PostPostLoad:
		_MESSAGE("Requesting an interface from CSE");
		g_msgIntfc->Dispatch(g_pluginHandle, 'CSEI', NULL, 0, "CSE");

		break;
	}
}

extern "C"
{
	bool OBSEPlugin_Query(const OBSEInterface * obse, PluginInfo * info)
	{
		info->infoVersion = PluginInfo::kInfoVersion;
		info->name = "REID";
		info->version = PACKED_SME_VERSION;

		if(obse->obseVersion < OBSE_VERSION_INTEGER)
		{
			_ERROR("OBSE version too old (got %08X expected at least %08X)", obse->obseVersion, OBSE_VERSION_INTEGER);
			return false;
		}
		else if(obse->isEditor == false)
		{
			if(obse->oblivionVersion != OBLIVION_VERSION)
			{
				_ERROR("incorrect Oblivion version (got %08X need %08X)", obse->oblivionVersion, OBLIVION_VERSION);
				return false;
			}

			g_strVarInfc = (OBSEStringVarInterface*)obse->QueryInterface(kInterface_StringVar);
			RegisterStringVarInterface(g_strVarInfc);

			if (g_strVarInfc == NULL)
			{
				_ERROR("Couldn't initialize string var interface");
				return false;
			}
		}

		g_msgIntfc = (OBSEMessagingInterface*)obse->QueryInterface(kInterface_Messaging);
		if (g_msgIntfc == NULL)
		{
			_ERROR("Couldn't initialize messaging interface");
			return false;
		}

		return true;
	}

	bool OBSEPlugin_Load(const OBSEInterface * obse)
	{
		g_pluginHandle = obse->GetPluginHandle();
		_MESSAGE("RuntimeEditorIDs Initializing...");
		gLog.Indent();

		if(!obse->isEditor)
		{
			for (int i = 0; i < VTBLTableSize; i++)
			{
				UInt32 PatchAddress = g_VTBLTable[i].Address + kTESForm_SetEditorID_VTBLOffset;
				SafeWrite32(PatchAddress, (UInt32)TESForm_HandleEditorID);

				_MESSAGE("Patched '%s' VTBL offset 0x%08X", g_VTBLTable[i].Class, PatchAddress);
			}
		}

		obse->SetOpcodeBase(0x2740);		// 0x2740 - 0x274F
		obse->RegisterTypedCommand(&kCommandInfo_GetRuntimeEditorID, kRetnType_String);

		g_msgIntfc->RegisterListener(g_pluginHandle, "OBSE", OBSEMessageHandler);

		gLog.Outdent();
		_MESSAGE("RuntimeEditorIDs Initialized!\n\n");
		return true;
	}
};