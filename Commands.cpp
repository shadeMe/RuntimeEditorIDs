#include "Commands.h"

static bool Cmd_GetRuntimeEditorID_Execute(COMMAND_ARGS)
{
	TESForm* BaseObject = NULL;

	if (!g_scriptIntfc->ExtractArgsEx(paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList, &BaseObject) ||
		(thisObj == NULL && BaseObject == NULL))
	{
		g_strVarInfc->Assign(PASS_COMMAND_ARGS, "");
		return true;
	}

	UInt32 FormID = 0;
	if (thisObj)
		FormID = thisObj->refID;
	else if (BaseObject)
		FormID = BaseObject->refID;

	const char* EditorID = g_editorIDManager.LookupByFormID(FormID);
	if (EditorID == NULL)
		g_strVarInfc->Assign(PASS_COMMAND_ARGS, "");
	else
		g_strVarInfc->Assign(PASS_COMMAND_ARGS, EditorID);

	if (EditorID && IsConsoleMode())
		Console_Print("EditorID: %s", EditorID);

	return true;
}

static ParamInfo kParams_GetRuntimeEditorID[1] =
{
	{	"base object",	kParamType_InventoryObject,	1	},
};

CommandInfo kCommandInfo_GetRuntimeEditorID =
{
	"GetRuntimeEditorID",
	"",
	0,
	"Returns the editorID of form",
	0,
	1,
	kParams_GetRuntimeEditorID,

	Cmd_GetRuntimeEditorID_Execute
};