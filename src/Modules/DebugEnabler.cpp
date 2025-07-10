// Original version written by Xx jAmes t xX and released here
// https://www.obscuregamers.com/threads/release-17559-hvp-xex-allows-debug-builds-to-run-on-jtag-rgh-without-rgl-or-xdk.1799/post-28644.html
//
// This version is very simplified and might not work for all games but it works for
// the games supported by this plugin

#include "pch.h"
#include "Modules/DebugEnabler.h"

#include "Core/Plugin.h"

namespace DebugEnabler
{

static Detour *s_pXexpResolveImageImportsDetour = nullptr;
static uint32_t s_DefaultValueAtPatchAddress = 0;

static int XexpResolveImageImportsHook(void *pExportBaseAddress, XEX_IMPORT_DESCRIPTOR *pImportDesc, uint32_t flags)
{
    XASSERT(s_pXexpResolveImageImportsDetour != nullptr);
    XASSERT(g_pPlugin != nullptr);

    // Get a pointer to the first string in the name table. The name table is right after
    // the import descriptor and contains moduleCount strings
    char *currentModuleName = reinterpret_cast<char *>(reinterpret_cast<uintptr_t>(pImportDesc) + sizeof(XEX_IMPORT_DESCRIPTOR));

    for (size_t i = 0; i < pImportDesc->ModuleCount; i++)
    {
        size_t currentModuleNameSize = strnlen_s(currentModuleName, MAX_PATH);
        size_t spaceTakenByCurrentModuleName = Math::RoundToNextMultipleOf<4>(currentModuleNameSize);

        // Look for xbdm.xex
        if (!strnicmp(currentModuleName, "xbdm.xex", currentModuleNameSize))
        {
            // Make sure we have enough space
            std::string pluginName = g_pPlugin->GetName();
            size_t requiredSpace = pluginName.size() + 1; // + 1 for the null character
            if (requiredSpace > spaceTakenByCurrentModuleName)
            {
                DebugPrint(
                    "[Hayzen][DebugEnabler]: Error: Not enough space to store the plugin name (%s),"
                    "required: %d, available: %d.",
                    pluginName.c_str(),
                    requiredSpace,
                    spaceTakenByCurrentModuleName
                );
                break;
            }

            // Replace xbdm.xex with the plugin name
            ZeroMemory(currentModuleName, currentModuleNameSize);
            strncpy_s(
                currentModuleName,
                requiredSpace,
                pluginName.c_str(),
                _TRUNCATE
            );

            break;
        }

        // Make the pointer point to the next string in the table
        currentModuleName += spaceTakenByCurrentModuleName;
    }

    return s_pXexpResolveImageImportsDetour->GetOriginal<decltype(&XexpResolveImageImportsHook)>()(pExportBaseAddress, pImportDesc, flags);
}

HRESULT Enable()
{
    XASSERT(s_pXexpResolveImageImportsDetour == nullptr);

    const uint64_t patchAddress = 0x800001040002AA58;

    s_DefaultValueAtPatchAddress = Hypervisor::Peek<uint32_t>(patchAddress);
    Hypervisor::Poke<uint32_t>(patchAddress, 0x60000000);

    s_pXexpResolveImageImportsDetour = new Detour(0x80079D48, XexpResolveImageImportsHook);

    return s_pXexpResolveImageImportsDetour->Install();
}

void Disable()
{
    XASSERT(s_pXexpResolveImageImportsDetour != nullptr);
    XASSERT(s_DefaultValueAtPatchAddress != 0);

    Hypervisor::Poke<uint32_t>(0x800001040002AA58, s_DefaultValueAtPatchAddress);

    delete s_pXexpResolveImageImportsDetour;
    s_pXexpResolveImageImportsDetour = nullptr;
}

}

// Dummy functions exported by this plugin to replace the 373 exported by xbdm
// when allowing debug builds to run on retail kernel.

#define STUBFN(n) \
    HRESULT __declspec(dllexport) StubExport##n() \
    { \
        return S_OK; \
    }

STUBFN(1)
STUBFN(2)
STUBFN(3)
STUBFN(4)
STUBFN(5)
STUBFN(6)
STUBFN(7)
STUBFN(8)
STUBFN(9)
STUBFN(10)
STUBFN(11)
STUBFN(12)
STUBFN(13)
STUBFN(14)
STUBFN(15)
STUBFN(16)
STUBFN(17)
STUBFN(18)
STUBFN(19)
STUBFN(20)
STUBFN(21)
STUBFN(22)
STUBFN(23)
STUBFN(24)
STUBFN(25)
STUBFN(26)
STUBFN(27)
STUBFN(28)
STUBFN(29)
STUBFN(30)
STUBFN(31)
STUBFN(32)
STUBFN(33)
STUBFN(34)
STUBFN(35)
STUBFN(36)
STUBFN(37)
STUBFN(38)
STUBFN(39)
STUBFN(40)
STUBFN(41)
STUBFN(42)
STUBFN(43)
STUBFN(44)
STUBFN(45)
STUBFN(46)
STUBFN(47)
STUBFN(48)
STUBFN(49)
STUBFN(50)
STUBFN(51)
STUBFN(52)
STUBFN(53)
STUBFN(54)
STUBFN(55)
STUBFN(56)
STUBFN(57)
STUBFN(58)
STUBFN(59)
STUBFN(60)
STUBFN(61)
STUBFN(62)
STUBFN(63)
STUBFN(64)
STUBFN(65)
STUBFN(66)
STUBFN(67)
STUBFN(68)
STUBFN(69)
STUBFN(70)
STUBFN(71)
STUBFN(72)
STUBFN(73)
STUBFN(74)
STUBFN(75)
STUBFN(76)
STUBFN(77)
STUBFN(78)
STUBFN(79)
STUBFN(80)
STUBFN(81)
STUBFN(82)
STUBFN(83)
STUBFN(84)
STUBFN(85)
STUBFN(86)
STUBFN(87)
STUBFN(88)
STUBFN(89)
STUBFN(90)
STUBFN(91)
STUBFN(92)
STUBFN(93)
STUBFN(94)
STUBFN(95)
STUBFN(96)
STUBFN(97)
STUBFN(98)
STUBFN(99)
STUBFN(100)
STUBFN(101)
STUBFN(102)
STUBFN(103)
STUBFN(104)
STUBFN(105)
STUBFN(106)
STUBFN(107)
STUBFN(108)
STUBFN(109)
STUBFN(110)
STUBFN(111)
STUBFN(112)
STUBFN(113)
STUBFN(114)
STUBFN(115)
STUBFN(116)
STUBFN(117)
STUBFN(118)
STUBFN(119)
STUBFN(120)
STUBFN(121)
STUBFN(122)
STUBFN(123)
STUBFN(124)
STUBFN(125)
STUBFN(126)
STUBFN(127)
STUBFN(128)
STUBFN(129)
STUBFN(130)
STUBFN(131)
STUBFN(132)
STUBFN(133)
STUBFN(134)
STUBFN(135)
STUBFN(136)
STUBFN(137)
STUBFN(138)
STUBFN(139)
STUBFN(140)
STUBFN(141)
STUBFN(142)
STUBFN(143)
STUBFN(144)
STUBFN(145)
STUBFN(146)
STUBFN(147)
STUBFN(148)
STUBFN(149)
STUBFN(150)
STUBFN(151)
STUBFN(152)
STUBFN(153)
STUBFN(154)
STUBFN(155)
STUBFN(156)
STUBFN(157)
STUBFN(158)
STUBFN(159)
STUBFN(160)
STUBFN(161)
STUBFN(162)
STUBFN(163)
STUBFN(164)
STUBFN(165)
STUBFN(166)
STUBFN(167)
STUBFN(168)
STUBFN(169)
STUBFN(170)
STUBFN(171)
STUBFN(172)
STUBFN(173)
STUBFN(174)
STUBFN(175)
STUBFN(176)
STUBFN(177)
STUBFN(178)
STUBFN(179)
STUBFN(180)
STUBFN(181)
STUBFN(182)
STUBFN(183)
STUBFN(184)
STUBFN(185)
STUBFN(186)
STUBFN(187)
STUBFN(188)
STUBFN(189)
STUBFN(190)
STUBFN(191)
STUBFN(192)
STUBFN(193)
STUBFN(194)
STUBFN(195)
STUBFN(196)
STUBFN(197)
STUBFN(198)
STUBFN(199)
STUBFN(200)
STUBFN(201)
STUBFN(202)
STUBFN(203)
STUBFN(204)
STUBFN(205)
STUBFN(206)
STUBFN(207)
STUBFN(208)
STUBFN(209)
STUBFN(210)
STUBFN(211)
STUBFN(212)
STUBFN(213)
STUBFN(214)
STUBFN(215)
STUBFN(216)
STUBFN(217)
STUBFN(218)
STUBFN(219)
STUBFN(220)
STUBFN(221)
STUBFN(222)
STUBFN(223)
STUBFN(224)
STUBFN(225)
STUBFN(226)
STUBFN(227)
STUBFN(228)
STUBFN(229)
STUBFN(230)
STUBFN(231)
STUBFN(232)
STUBFN(233)
STUBFN(234)
STUBFN(235)
STUBFN(236)
STUBFN(237)
STUBFN(238)
STUBFN(239)
STUBFN(240)
STUBFN(241)
STUBFN(242)
STUBFN(243)
STUBFN(244)
STUBFN(245)
STUBFN(246)
STUBFN(247)
STUBFN(248)
STUBFN(249)
STUBFN(250)
STUBFN(251)
STUBFN(252)
STUBFN(253)
STUBFN(254)
STUBFN(255)
STUBFN(256)
STUBFN(257)
STUBFN(258)
STUBFN(259)
STUBFN(260)
STUBFN(261)
STUBFN(262)
STUBFN(263)
STUBFN(264)
STUBFN(265)
STUBFN(266)
STUBFN(267)
STUBFN(268)
STUBFN(269)
STUBFN(270)
STUBFN(271)
STUBFN(272)
STUBFN(273)
STUBFN(274)
STUBFN(275)
STUBFN(276)
STUBFN(277)
STUBFN(278)
STUBFN(279)
STUBFN(280)
STUBFN(281)
STUBFN(282)
STUBFN(283)
STUBFN(284)
STUBFN(285)
STUBFN(286)
STUBFN(287)
STUBFN(288)
STUBFN(289)
STUBFN(290)
STUBFN(291)
STUBFN(292)
STUBFN(293)
STUBFN(294)
STUBFN(295)
STUBFN(296)
STUBFN(297)
STUBFN(298)
STUBFN(299)
STUBFN(300)
STUBFN(301)
STUBFN(302)
STUBFN(303)
STUBFN(304)
STUBFN(305)
STUBFN(306)
STUBFN(307)
STUBFN(308)
STUBFN(309)
STUBFN(310)
STUBFN(311)
STUBFN(312)
STUBFN(313)
STUBFN(314)
STUBFN(315)
STUBFN(316)
STUBFN(317)
STUBFN(318)
STUBFN(319)
STUBFN(320)
STUBFN(321)
STUBFN(322)
STUBFN(323)
STUBFN(324)
STUBFN(325)
STUBFN(326)
STUBFN(327)
STUBFN(328)
STUBFN(329)
STUBFN(330)
STUBFN(331)
STUBFN(332)
STUBFN(333)
STUBFN(334)
STUBFN(335)
STUBFN(336)
STUBFN(337)
STUBFN(338)
STUBFN(339)
STUBFN(340)
STUBFN(341)
STUBFN(342)
STUBFN(343)
STUBFN(344)
STUBFN(345)
STUBFN(346)
STUBFN(347)
STUBFN(348)
STUBFN(349)
STUBFN(350)
STUBFN(351)
STUBFN(352)
STUBFN(353)
STUBFN(354)
STUBFN(355)
STUBFN(356)
STUBFN(357)
STUBFN(358)
STUBFN(359)
STUBFN(360)
STUBFN(361)
STUBFN(362)
STUBFN(363)
STUBFN(364)
STUBFN(365)
STUBFN(366)
STUBFN(367)
STUBFN(368)
STUBFN(369)
STUBFN(370)
STUBFN(371)
STUBFN(372)
STUBFN(373)
