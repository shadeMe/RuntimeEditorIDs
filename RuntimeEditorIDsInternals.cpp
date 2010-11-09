#include "RuntimeEditorIDsInternals.h"

const NiTMapBase<const char*, TESForm *>* g_EditorIDTable = (const NiTMapBase<const char*, TESForm *>*)0x00B06164;

EditorIDManager				g_editorIDManager;

void EditorIDManager::Lookup(const char* LookupID, const char** ResultID, std::vector<TESForm*>** ResultFormList)
{
	_AllocMap::const_iterator Itr = AllocMap.find(const_cast<char*>(LookupID));
	if (Itr != AllocMap.end())
	{
		*ResultID = Itr->first;
		*ResultFormList = const_cast<std::vector<TESForm*>*>(Itr->second);
		return;
	}
	*ResultID = NULL;
	*ResultFormList = NULL;
}

void EditorIDManager::Manage(const char* EditorID, TESForm* Form)
{
	const char* AllocatedEditorID = NULL;
	std::vector<TESForm*>* FormList = NULL;

	Lookup(EditorID, &AllocatedEditorID, &FormList);

	if (!AllocatedEditorID)
	{
		_MESSAGE("Allocating editorID '%s' for form %08X", EditorID, Form->refID);

		UInt32 Length = strlen(EditorID);	
		char* AllocString = (char *)FormHeap_Allocate(Length + 1);
		strcpy_s(AllocString, Length + 1, EditorID);

		std::vector<TESForm*>* InitList = new std::vector<TESForm*>;
		InitList->push_back(Form);
	
		AllocMap.insert(std::make_pair<char*, std::vector<TESForm*>*>(AllocString, InitList));
		AllocatedEditorID = AllocString;
	}
	else
	{
		_MESSAGE("Overriding editorID '%s' with form %08X", EditorID, Form->refID);

		FormList->push_back(Form);
	}

	ThisStdCall(kNiTStringPointerMap_Add, (void*)g_EditorIDTable, AllocatedEditorID, Form);
}

void __stdcall TESForm_HandleEditorID(const char* EditorID)
{
	__asm
	{
		push	ecx
		push	EditorID
		lea		ecx, g_editorIDManager
		call	EditorIDManager::Manage
	}
}