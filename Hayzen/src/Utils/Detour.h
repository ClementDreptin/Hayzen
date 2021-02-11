#pragma once

#include <xtl.h>

/*	
Made by iMoD1998	
V2.3	
*/

class Detour
{
public:

	Detour( ) { };
	
	Detour( 
		_Inout_ void* Source,
		_In_    void* Destination,
		_In_    bool PreserveRegisters = true
	);
	
	~Detour( );

	void RelocateBranch( 
		_In_ DWORD* Instruction
	);

	void RelocateBranchConditional( 
		_In_ DWORD* Instruction
	);

	void RelocateCode(
		_In_ DWORD* Instruction
	);

	void PatchInJump( 
		_Inout_ void* Source,
		_In_    void* Destination,
		_In_    bool  Linked = true,
		_In_    bool  PreserveRegisters = true
	);
	
	void PatchInJumpEx( 
		_Inout_ void* Source,
		_In_    void* Destination,
		_In_    DWORD BranchOptions,
		_In_    DWORD ConditionRegister,
		_In_    bool  Linked = false,
		_In_    bool  PreserveRegisters = true
	);

	void* DetourFunction( 
		_Inout_ void* Source,
		_In_    void* Destination,
		_In_    bool  PreserveRegisters = true
	);

	void RestoreFunction(
		void
	);

	template <typename T>
	T __forceinline GetOriginal( )
	{
		return (T)(void*)this->StubInstructions;
	}

	void* HookCbFunction;

private:
	BYTE* StubInstructions;
	BYTE OriginalInstructions[ 30 ];
	void* HookAddress;
	SIZE_T OriginalLength;
	SIZE_T StubIndex;

	static BYTE Stubs[ 200 * 20 ];
	static SIZE_T StubOffset;
};