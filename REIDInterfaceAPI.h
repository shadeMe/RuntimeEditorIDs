#pragma once

/********** REID Interface API ******************************************************************
*	Dispatch a message of type 'EDID', its data pointing to an instance of the following struct
*	The struct's formID field must be filled beforehand.
*	The 'HasEditorID' will be set to true and the buffer populated if a valid editorID is found
*	If not, the former will be set to false
***********************************************************************************************/

struct REIDInteropData
{
	static const UInt32			kBufferSize = 0x200;

	UInt32						FormID;

	bool						HasEditorID;
	char						EditorIDBuffer[kBufferSize];
};
