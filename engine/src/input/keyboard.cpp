//
// Created by Andrew Huang on 7/12/2022.
//

#include "input/keyboard.h"

// Adapted from DirectXTK

// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkId=248929
// http://go.microsoft.com/fwlink/?LinkID=615561

// Modified by Andrew Huang

#include <Windows.h>

template<bool B>
static inline void SetState(uint32_t state[8], Keyboard::Key key) noexcept {
    if (key < 0 || key > 0xfe)
        return;

    const unsigned int mask = 1u << (key & 0x1f);
    if constexpr(B) {
        state[(key >> 5)] |= mask;
    } else {
        state[(key >> 5)] &= ~mask;
    }
}

void Keyboard::ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam) {
    auto &keyboard = Keyboard::Get();

    bool down = false;

    switch (message) {
    case WM_ACTIVATE:
    case WM_ACTIVATEAPP:
        keyboard.Reset();
        return;

    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        down = true;
        break;

    case WM_KEYUP:
    case WM_SYSKEYUP:
        // ignored
    default:
        break;
    }

    int vk = LOWORD(wParam);
    // We want to distinguish left and right shift/ctrl/alt keys
    switch (vk) {
    case VK_SHIFT:
    case VK_CONTROL:
    case VK_MENU: {
        if (vk == VK_SHIFT && !down) {
            // Workaround to ensure left vs. right shift get cleared when both were pressed at same time
            SetState<false>(keyboard.m_state, static_cast<Key>(VK_LSHIFT));
            SetState<false>(keyboard.m_state, static_cast<Key>(VK_RSHIFT));
        }

        bool isExtendedKey = (HIWORD(lParam) & KF_EXTENDED) == KF_EXTENDED;
        int  scanCode      = LOBYTE(HIWORD(lParam)) | (isExtendedKey ? 0xe000 : 0);
        vk = LOWORD(MapVirtualKeyW(static_cast<UINT>(scanCode), MAPVK_VSC_TO_VK_EX));
    }
        break;
    default:
        break;
    }

    if (down) {
        SetState<true>(keyboard.m_state, static_cast<Key>(vk));
    } else {
        SetState<false>(keyboard.m_state, static_cast<Key>(vk));
    }
}

void Keyboard::Reset() {
    ZeroMemory(m_state, sizeof(State));
}

void Keyboard::InternalUpdate() {
    const uint32_t *currPtr = m_state;
    const uint32_t *prevPtr = m_lastState;

    auto releasedPtr = m_released;
    auto pressedPtr  = m_pressed;

    for (size_t j = 0; j < (256 / 32); ++j) {
        *pressedPtr  = *currPtr & ~(*prevPtr);
        *releasedPtr = ~(*currPtr) & *prevPtr;

        ++currPtr;
        ++prevPtr;
        ++releasedPtr;
        ++pressedPtr;
    }

    memcpy(m_lastState, m_state, sizeof(State));
}
