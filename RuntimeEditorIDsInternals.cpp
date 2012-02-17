#include "RuntimeEditorIDsInternals.h"

const NiTMapBase<const char*, TESForm *>* g_EditorIDTable = (const NiTMapBase<const char*, TESForm *>*)0x00B06164;

EditorIDManager				g_editorIDManager;

void EditorIDManager::LookupByEditorID(const char* LookupID, const char** ResultID, std::vector<TESForm*>** ResultFormList)
{
	EditorIDAllocationMapT::const_iterator Itr = EIDAllocationMap.find(const_cast<char*>(LookupID));
	if (Itr != EIDAllocationMap.end())
	{
		*ResultID = Itr->first;
		*ResultFormList = const_cast<std::vector<TESForm*>*>(Itr->second);
		return;
	}
	*ResultID = NULL;
	*ResultFormList = NULL;
}

const char* EditorIDManager::LookupByFormID(UInt32 FormID)
{
	FormIDEditorIDMapT::const_iterator Itr = FormIDReferenceMap.find(FormID);
	if (Itr != FormIDReferenceMap.end())
		return Itr->second;
	else
		return NULL;
}

void EditorIDManager::Manage(const char* EditorID, TESForm* Form)
{
	const char* AllocatedEditorID = NULL;
	std::vector<TESForm*>* FormList = NULL;

	LookupByEditorID(EditorID, &AllocatedEditorID, &FormList);

	if (!AllocatedEditorID)
	{
		UInt32 Length = strlen(EditorID);
		char* AllocString = (char *)FormHeap_Allocate(Length + 1);
		strcpy_s(AllocString, Length + 1, EditorID);

		std::vector<TESForm*>* InitList = new std::vector<TESForm*>;
		InitList->push_back(Form);

		EIDAllocationMap.insert(std::make_pair<char*, std::vector<TESForm*>*>(AllocString, InitList));
		AllocatedEditorID = AllocString;
	}
	else
	{
		FormList->push_back(Form);
	}

	FormIDReferenceMap[Form->refID] = AllocatedEditorID;

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