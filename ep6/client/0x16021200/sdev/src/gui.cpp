#include <array>

#include <include/util.h>
#include <include/main.h>

const std::array<std::uint16_t, 21> weapon_step
{
    0, 7, 14, 21, 31, 41, 51, 64, 77, 90, 106, 122, 138, 157, 176, 195, 217, 239, 261, 286, 311
};

int get_weapon_step(std::uint8_t step)
{
    if (step >= weapon_step.size())
        return 0;

    return weapon_step[step];
}

unsigned u0x4B8766 = 0x4B8766;
void __declspec(naked) naked_0x4B8755()
{
    __asm
    {
        push eax // step
        call get_weapon_step
        add esp,0x4

        jmp u0x4B8766
    }
}

unsigned u0x59F8AF = 0x59F8AF;
void __declspec(naked) naked_0x59F896()
{
    __asm
    {
        // CCharacter*
        cmp esi,dword ptr ds:[0x904AA0]
        jne wrong_appearance

        // sex, size, face, hair
        mov byte ptr ds:[0x914474],al
        mov byte ptr ds:[0x913472],cl
        mov byte ptr ds:[0x913471],dl
        mov byte ptr ds:[0x913470],bl

        wrong_appearance:
        mov ecx,esi
        jmp u0x59F8AF
    }
}

void hook::gui()
{
    // weapon enchant bug
    util::detour((void*)0x4B8755, naked_0x4B8755, 5);
    // appearance/sex change bug
    util::detour((void*)0x59F896, naked_0x59F896, 6);
    // move when using cash shop
    std::array<std::uint8_t, 2> a00{ 0xEB, 0x2A };
    std::memcpy((void*)0x44565D, &a00, 2);
    // costume lag workaround
    std::array<std::uint8_t, 2> a01{ 0x75, 0x2F };
    std::memcpy((void*)0x56F38D, &a01, 2);
    std::array<std::uint8_t, 2> a02{ 0x75, 0x1E };
    std::memcpy((void*)0x583DED, &a02, 2);
    // pet/wing lag workaround
    std::array<std::uint8_t, 6> a03{ 0x0F, 0x85, 0x84, 0x00, 0x00, 0x00 };
    std::memcpy((void*)0x5881ED, &a03, 6);
    // remove ep6 vehicles (auction board)
    std::array<std::uint8_t, 1> a04{ 0x07 };
    std::memcpy((void*)0x463FE0, &a04, 1);

    // 0x2602 EP5 packet format

    std::array<std::uint8_t, 3> a05{ 0x33, 0xD2, 0x90 };
    std::array<std::uint8_t, 3> a06{ 0x33, 0xC0, 0x90 };
    // ignore and null the dates
    util::write_memory((void*)0x5A3DAC, &a05, 3);
    util::write_memory((void*)0x5A3DB5, &a06, 3);

    std::uint8_t item_def_size = 5;
    // overwrite 13 with 5
    util::write_memory((void*)0x5E9841, &item_def_size, 1);
    util::write_memory((void*)0x5A3DE6, &item_def_size, 1);
}
