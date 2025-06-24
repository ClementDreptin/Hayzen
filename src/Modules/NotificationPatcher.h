#pragma once

// Notifications don't work when created from system threads, this module patches XAM to allow it.
//
// The same patch is usually applied by RPC servers (JRPC2, XDRPC, etc.) and most RGH/Jtag users have one loaded,
// but most Devkit and BadUpdate users don't, so this module makes sure notifications work for everyone.

namespace NotificationPatcher
{

void Enable();

void Disable();

}
