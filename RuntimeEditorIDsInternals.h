#pragma once

#include <map>
#include <vector>

#include "CommandTable.h"
#include "obse_common\SafeWrite.h"
#include "obse\PluginAPI.h"
#include "obse\GameAPI.h"
#include "obse\GameTypes.h"
#include "obse\ParamInfos.h"
#include "obse\ScriptUtils.h"
#include "obse\GameForms.h"
#include "obse\GameObjects.h"

#include <SME_Prefix.h>
#include <MiscGunk.h>

#include "REIDInterfaceAPI.h"

#define CSEAPI_NO_CODA
#include "Construction Set Extender\CSEInterfaceAPI.h"

#if OBLIVION_VERSION == OBLIVION_VERSION_1_2_416
	extern const NiTMapBase<const char*, TESForm *>* g_EditorIDTable;	// BSTCaseInsensitiveStringMap<TESForm*>*

	const UInt32 kNiTPointerMap_Add = 0x00452570;			// f(this, T1 key, T2 value)
	const UInt32 kNiTPointerMap_Remove = 0x004524C0;		// f(this, T1 key)
	const UInt32 kNiTPointerMap_Lookup = 0x0055E000;		// f(this, T1 key, T2* foundItem)

	const UInt32 kNiTStringPointerMap_Add = 0x00412D30;		// f(this, const char* key, T1 value)
	const UInt32 kTESForm_SetEditorID_VTBLOffset = 0xD8;
	const UInt32 kTESForm_GetEditorID_VTBLOffset = 0xD4;

	struct VTBLData
	{
		UInt32				Address;
		const char*			Class;
	};

	const UInt32 VTBLTableSize = 53;
	const VTBLData g_VTBLTable[VTBLTableSize] =
	{
		{ 0x00A52C6C, "TESClass" },
		{ 0x00A53524, "TESFaction" },
		{ 0x00A53654, "TESHair" },
		{ 0x00A53414, "TESEyes" },
		{ 0x00A548FC, "TESRace" },
		{ 0x00A54EFC, "TESSkill" },
		{ 0x00A32B14, "EffectSetting" },
		{ 0x00A49DA4, "Script" },
		{ 0x00A4656C, "TESLandTexture" },
		{ 0x00A33C84, "EnchantmentItem" },
		{ 0x00A34C3C, "SpellItem" },
		{ 0x00A52524, "BirthSign" },
		{ 0x00A43CB4, "TESObjectACTI" },
		{ 0x00A43FB4, "TESObjectAPPA" },
		{ 0x00A441BC, "TESObjectARMO" },
		{ 0x00A44454, "TESObjectBOOK" },
		{ 0x00A44644, "TESObjectCLOT" },
		{ 0x00A44824, "TESObjectCONT" },
		{ 0x00A44A54, "TESObjectDOOR" },
		{ 0x00A33F5C, "IngredientItem" },
		{ 0x00A4319C, "TESObjectLIGH" },
		{ 0x00A44CA4, "TESObjectMISC" },
		{ 0x00A44DEC, "TESObjectSTAT" },
		{ 0x00A4280C, "TESGrass" },
		{ 0x00A44FB4, "TESObjectTREE" },
		{ 0x00A41B1C, "TESFlora" },
		{ 0x00A41D1C, "TESFurniture" },
		{ 0x00A45354, "TESObjectWEAP" },
		{ 0x00A4068C, "TESAmmo" },
		{ 0x00A53DD4, "TESNPC" },
		{ 0x00A5324C, "TESCreature" },
		{ 0x00A42B9C, "TESLevCreature" },
		{ 0x00A456F4, "TESSoulGem" },
		{ 0x00A42A4C, "TESKey" },
		{ 0x00A3217C, "AlchemyItem" },
		{ 0x00A4592C, "TESSubSpace" },
		{ 0x00A45534, "TESSigilStone" },
		{ 0x00A42DC4, "TESLevItem" },
		{ 0x00A47FEC, "TESWeather" },
		{ 0x00A45C9C, "TESClimate" },
		{ 0x00A3FEE4, "TESRegion" },
		{ 0x00A46C44, "TESObjectREFR" },
		{ 0x00A53774, "TESIdleForm" },
		{ 0x00A6743C, "TESPackage" },
		{ 0x00A407F4, "TESCombatStyle" },
		{ 0x00A4997C, "TESLoadScreen" },
		{ 0x00A42F1C, "TESLevSpell" },
		{ 0x00A43DFC, "TESObjectANIO" },
		{ 0x00A47F0C, "TESWaterForm" },
		{ 0x00A419EC, "TESEffectShader" },
		{ 0x00A6FC9C, "Character" },
		{ 0x00A6E074, "Actor" },
		{ 0x00A710F4, "Creature" }
	};

#else
	#error unsupported oblivion version
#endif

class EditorIDManager
{
	struct CaseInsensitiveStringComparer
	{
		bool operator()(const char* Key1, const char* Key2) const
		{
			return _stricmp(Key1, Key2) < 0;
		}
	};

	typedef std::map<char*, std::vector<TESForm*>*, CaseInsensitiveStringComparer>		EditorIDAllocationMapT;
	typedef std::map<UInt32, const char*>												FormIDEditorIDMapT;

	EditorIDAllocationMapT								EIDAllocationMap;
	FormIDEditorIDMapT									FormIDReferenceMap;

	void												LookupByEditorID(const char* LookupID, const char** ResultID, std::vector<TESForm*>** ResultFormList);
public:
	void												Manage(const char* EditorID, TESForm* Form);
	const char*											LookupByFormID(UInt32 FormID);
};
extern EditorIDManager		g_editorIDManager;

void __stdcall TESForm_HandleEditorID(const char* EditorID);

extern OBSEStringVarInterface*		g_strVarInfc;
extern OBSEMessagingInterface*		g_msgIntfc;
extern OBSEScriptInterface*			g_scriptIntfc;
