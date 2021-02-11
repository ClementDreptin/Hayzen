#include "pch.h"
#include "Detour.h"
#include <stdio.h>

#define ADDRESS_HIGHER(X) ( ( X >> 16 ) & 0xFFFF )
#define ADDRESS_LOWER(X)  ( X & 0xFFFF )

#define POWERPC_DETOUR_SIZE sizeof(JumpASMPreserve)
#define POWERPC_INSTRUCTION_LENGTH 4

/*Opcodes*/
#define POWERPC_BRANCH             0x48000000
#define POWERPC_BRANCH_CONDITIONAL 0x40000000

/*Takes Human based syntax numbers and returns approiate bits ie. bcctr 20, 0*/
#define POWERPC_BRANCH_OPTION_SET(X)      ( X << 21 ) 
#define POWERPC_CONDITION_REGISTER_SET(X) ( X << 16 )

/*Takes powerpc bits and converts to human based syntax*/
#define POWERPC_BRANCH_OPTION_GET(X)      ( X >> 21 )
#define POWERPC_CONDITION_REGISTER_GET(X) ( X >> 16 )

const DWORD JumpASMPreserve[] = {
	0x9161FFD0, // stw %r11, -0x30(%r1)
	0x3D600000, // lis %r11, 0
	0x616B0000, // ori %r11, %r11, 0
	0x7D6903A6, // mtctr %r11
	0x8161FFD0, // lwz %r11, -0x30(%r1)
	0x4C800420  // bcctr (Used For PatchInJumpEx bcctr 20, 0 == bctr)
};

const DWORD JumpASMNoPreserve[] = { /*Dont always have enough space to preserve registers*/
	0x3d600000, // lis %r11, Destination@h
	0x616b0000, // ori %r11, %r11, Destination@l
	0x7d6903a6, // mtctr %r11
	0x4C800420  // bcctr (Used For PatchInJumpEx bcctr 20, 0 == bctr)
};

unsigned char Detour::Stubs[] = { 0 };
SIZE_T Detour::StubOffset = 0;

void Detour::RelocateBranch(
	_In_ DWORD* Instruction
)
{
	/*

	B - Branch

	[Opcode]       [Address]              [Absolute] [Linked]
	0-5              6-29                     30        31

	Example

	‭010010    0000 0000 0000 0000 0000 0001   0         0‬

	b label
	label:

	*/

	const auto InstructionAddress = (DWORD)Instruction;
	const auto InstructionOpcode = *(DWORD*)Instruction;
	const auto CurrentStubPos = (DWORD*)&this->StubInstructions[ this->StubIndex ];

	if ( InstructionOpcode & 0x2 ) /*BA BLA*/
	{
		*CurrentStubPos = InstructionOpcode;
		this->StubIndex += POWERPC_INSTRUCTION_LENGTH;
	}
	else
	{
		INT_PTR BranchOffset = InstructionOpcode & 0x03FFFFFC;

		if ( BranchOffset & ( 1 << 25 ) ) // If The MSB Is Set Make It Negative
			BranchOffset |= ~0x03FFFFFF;

		auto OriginalAddress = (intptr_t)( InstructionAddress + BranchOffset );

		this->PatchInJump( CurrentStubPos, (void*)OriginalAddress, InstructionOpcode & 1 );

		this->StubIndex += POWERPC_DETOUR_SIZE;
	}
}

void Detour::RelocateBranchConditional(
	_In_ DWORD* Instruction
)
{
	/*

	BC - Branch Conditional

	[Opcode]   [Branch Options]     [Condition Register]         [Address]      [Absolute] [Linked]
	0-5            6-10                    11-15                   16-29            30        31

	Example
	010000         00100                   00001              0000 0000 0000 01      0         0‬

	*/

	const auto InstructionAddress = (DWORD)Instruction;
	const auto InstructionOpcode  = *(DWORD*)Instruction;
	const auto CurrentStubPos     = (DWORD*)&this->StubInstructions[ this->StubIndex ];

	if ( InstructionOpcode & 0x2 ) /*Condtional Absolute*/
	{
		*CurrentStubPos = InstructionOpcode;
		this->StubIndex += POWERPC_INSTRUCTION_LENGTH;
	}
	else
	{
		const auto BranchOptions     = InstructionOpcode & 0x03E00000;
		const auto ConditionRegister = InstructionOpcode & 0x001F0000;

		INT_PTR BranchOffset = InstructionOpcode & 0x0000FFFC;

		if ( BranchOffset & ( 1 << 15 ) ) // If The MSB Is Set Make It Negative
			BranchOffset |= ~0x0000FFFF;

		auto OriginalAddress = (INT_PTR)( InstructionAddress + BranchOffset );

		this->PatchInJumpEx( (void*)CurrentStubPos, (void*)OriginalAddress, POWERPC_BRANCH_OPTION_GET( BranchOptions ), POWERPC_CONDITION_REGISTER_GET( ConditionRegister ), InstructionOpcode & 1, true );

		this->StubIndex += POWERPC_DETOUR_SIZE;
	}
}

void Detour::RelocateCode(
	_In_ DWORD* Instruction
)
{
	const auto InstructionOpcode = *(DWORD*)Instruction;
	const auto CurrentStubPos = ( DWORD* )&this->StubInstructions[ this->StubIndex ];

	switch ( InstructionOpcode & 0xFC000000 )
	{
		case POWERPC_BRANCH: /*B BL BA BLA*/
			Detour::RelocateBranch( Instruction );
			break;

		case POWERPC_BRANCH_CONDITIONAL: /*BEQ BNE BLT BGE */
			Detour::RelocateBranchConditional( Instruction );
			break;

		default:
			*CurrentStubPos = InstructionOpcode;
			this->StubIndex += POWERPC_INSTRUCTION_LENGTH;
			break;
	}
}

void Detour::PatchInJump(
	_Inout_ void* Source,
	_In_    void* Destination,
	_In_    bool  Linked,
	_In_    bool  PreserveRegisters
)
{
	this->PatchInJumpEx( Source, Destination, 20, 0, Linked, PreserveRegisters );
}

void Detour::PatchInJumpEx(
	_Inout_ void* Source,
	_In_    void* Destination,
	_In_    DWORD BranchOptions,
	_In_    DWORD ConditionRegister,
	_In_    bool  Linked,
	_In_    bool  PreserveRegisters
)
{
	BranchOptions &= 0x1F; // Options only takes 5 bits
	ConditionRegister &= 0x1F;

	const auto DestinationAddress = (uintptr_t)Destination;

	if ( PreserveRegisters )
	{
		DWORD Instructions[ ARRAYSIZE( JumpASMPreserve ) ];

		memcpy( Instructions, JumpASMPreserve, sizeof( JumpASMPreserve ) );

		Instructions[ 1 ] |= ADDRESS_HIGHER( DestinationAddress );
		Instructions[ 2 ] |= ADDRESS_LOWER( DestinationAddress );
		Instructions[ 5 ] |= POWERPC_BRANCH_OPTION_SET( BranchOptions ) | POWERPC_CONDITION_REGISTER_SET( ConditionRegister ) | (DWORD)Linked;

		memcpy( ( void* )this->OriginalInstructions, Source, sizeof( JumpASMPreserve ) );
		memcpy( Source, Instructions, sizeof( Instructions ) );

		this->OriginalLength = POWERPC_DETOUR_SIZE;
	}
	else
	{
		DWORD Instructions[ ARRAYSIZE( JumpASMNoPreserve ) ];

		memcpy( Instructions, JumpASMNoPreserve, sizeof( JumpASMNoPreserve ) );

		Instructions[ 0 ] |= ADDRESS_HIGHER( DestinationAddress );
		Instructions[ 1 ] |= ADDRESS_LOWER( DestinationAddress );
		Instructions[ 3 ] |= POWERPC_BRANCH_OPTION_SET( BranchOptions ) | POWERPC_CONDITION_REGISTER_SET( ConditionRegister ) | (DWORD)Linked;

		memcpy( ( void* )this->OriginalInstructions, Source, sizeof( JumpASMNoPreserve ) );
		memcpy( Source, Instructions, sizeof( Instructions ) );

		this->OriginalLength = sizeof(JumpASMNoPreserve);
	}

	this->HookAddress = Source;
}

void* Detour::DetourFunction(
	_Inout_ void* Source,
	_In_    void* Destination,
	_In_    bool  PreserveRegisters
)
{
	this->StubInstructions = &Detour::Stubs[ Detour::StubOffset ];

	const auto SourceAddress = (UINT_PTR)Source;

	for ( int i = 0; i < ARRAYSIZE( JumpASMPreserve ); ++i )
	{
		auto InstructionAddress = ( SourceAddress + ( i * POWERPC_INSTRUCTION_LENGTH ) );

		this->RelocateCode( (DWORD*)InstructionAddress );
	}

	const auto HookSize = PreserveRegisters ? sizeof( JumpASMPreserve ) : sizeof( JumpASMNoPreserve );

	const auto AfterJumpAddress = (DWORD)( SourceAddress + HookSize );

	this->PatchInJump( ( void* )&this->StubInstructions[ this->StubIndex ], (void*)AfterJumpAddress, false, true );

	this->PatchInJump( Source, Destination, false, PreserveRegisters );

	// gotta add on the size of the last jump back to the start of the orginal function
	Detour::StubOffset += this->StubIndex + sizeof( JumpASMPreserve );

	this->HookCbFunction = Destination;

	return ( void* )this->StubInstructions;
}

void Detour::RestoreFunction(
	void
)
{
	if ( this->OriginalLength )
	{
		memcpy( ( void* )this->HookAddress, ( void* )this->OriginalInstructions, this->OriginalLength );

		this->OriginalLength = 0;

		Sleep( 100 );
	}
}

Detour::Detour(
	_Inout_ void* Source,
	_In_    void* Destination,
	_In_    bool PreserveRegisters
)
{
	this->DetourFunction( Source, Destination, PreserveRegisters );
}

Detour::~Detour( )
{
	//this->RestoreFunction( );
}