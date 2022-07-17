//
// Created by andyroiiid on 7/16/2022.
//

#include "state/terminal.h"

#include <input/keyboard.h>
#include <logging.h>

#include "state/log_viewer.h"

Terminal::Terminal() {
    SetTitle("Terminal");
}

void Terminal::OnResize(const IntVec2 &size) {
    m_size = Vec2(size);
    glViewport(0, 0, size.x, size.y);
    m_textRenderer.OnResize(size);
}

struct KeyToChar {
    Keyboard::Key key;
    char          normal;
    char          caps;
    char          shift;
    char          capsAndShift;
};

static const KeyToChar INPUT_MAPPINGS[] = {
        {Keyboard::A,                'a',  'A',  'A', 'a'},
        {Keyboard::B,                'b',  'B',  'B', 'b'},
        {Keyboard::C,                'c',  'C',  'C', 'c'},
        {Keyboard::D,                'd',  'D',  'D', 'd'},
        {Keyboard::E,                'e',  'E',  'E', 'e'},
        {Keyboard::F,                'f',  'F',  'F', 'f'},
        {Keyboard::G,                'g',  'G',  'G', 'g'},
        {Keyboard::H,                'h',  'H',  'H', 'h'},
        {Keyboard::I,                'i',  'I',  'I', 'i'},
        {Keyboard::J,                'j',  'J',  'J', 'j'},
        {Keyboard::K,                'k',  'K',  'K', 'k'},
        {Keyboard::L,                'l',  'L',  'L', 'l'},
        {Keyboard::M,                'm',  'M',  'M', 'm'},
        {Keyboard::N,                'n',  'N',  'N', 'n'},
        {Keyboard::O,                'o',  'O',  'O', 'o'},
        {Keyboard::P,                'p',  'P',  'P', 'p'},
        {Keyboard::Q,                'q',  'Q',  'Q', 'q'},
        {Keyboard::R,                'r',  'R',  'R', 'r'},
        {Keyboard::S,                's',  'S',  'S', 's'},
        {Keyboard::T,                't',  'T',  'T', 't'},
        {Keyboard::U,                'u',  'U',  'U', 'u'},
        {Keyboard::V,                'v',  'V',  'V', 'v'},
        {Keyboard::W,                'w',  'W',  'W', 'w'},
        {Keyboard::X,                'x',  'X',  'X', 'x'},
        {Keyboard::Y,                'y',  'Y',  'Y', 'y'},
        {Keyboard::Z,                'z',  'Z',  'Z', 'z'},

        {Keyboard::Space,            ' ',  ' ',  ' ', ' '},

        {Keyboard::D1,               '1',  '1',  '!', '!'},
        {Keyboard::D2,               '2',  '2',  '@', '@'},
        {Keyboard::D3,               '3',  '3',  '#', '#'},
        {Keyboard::D4,               '4',  '4',  '$', '$'},
        {Keyboard::D5,               '5',  '5',  '%', '%'},
        {Keyboard::D6,               '6',  '6',  '^', '^'},
        {Keyboard::D7,               '7',  '7',  '&', '&'},
        {Keyboard::D8,               '8',  '8',  '*', '*'},
        {Keyboard::D9,               '9',  '9',  '(', '('},
        {Keyboard::D0,               '0',  '0',  ')', ')'},

        {Keyboard::OemSemicolon,     ';',  ';',  ':', ':'},
        {Keyboard::OemPlus,          '=',  '=',  '+', '+'},
        {Keyboard::OemComma,         ',',  ',',  '<', '<'},
        {Keyboard::OemMinus,         '-',  '-',  '_', '_'},
        {Keyboard::OemPeriod,        '.',  '.',  '>', '>'},
        {Keyboard::OemQuestion,      '/',  '/',  '?', '?'},
        {Keyboard::OemTilde,         '`',  '`',  '~', '~'},

        {Keyboard::OemOpenBrackets,  '[',  '[',  '{', '{'},
        {Keyboard::OemPipe,          '\\', '\\', '|', '|'},
        {Keyboard::OemCloseBrackets, ']',  ']',  '}', '}'},
        {Keyboard::OemQuotes,        '\'', '\'', '"', '"'},
};

Proto2D::StateBuilder Terminal::Update(float deltaTime) {
    m_fps = 1.0f / deltaTime;

    const bool caps  = Keyboard::GetCapsLock();
    const bool shift = Keyboard::GetKey(Keyboard::LeftShift) || Keyboard::GetKey(Keyboard::RightShift);

    for (const auto mapping: INPUT_MAPPINGS) {
        if (Keyboard::GetKeyDown(mapping.key)) {
            if (caps) {
                m_input.push_back(shift ? mapping.capsAndShift : mapping.caps);
            } else {
                m_input.push_back(shift ? mapping.shift : mapping.normal);
            }
        }
    }

    if (!m_input.empty()) {
        if (Keyboard::GetKeyDown(Keyboard::Back)) {
            m_input.pop_back();
        }

        if (Keyboard::GetKeyDown(Keyboard::Enter)) {
            m_input.push_back('\0');
            LogInfo("%s", m_input.data());
            m_input.clear();
        }
    }

    if (Keyboard::GetKeyDown(Keyboard::Tab)) {
        return []() -> std::unique_ptr<Proto2D::State> {
            return std::make_unique<LogViewer>();
        };
    } else {
        return nullptr;
    }
}

void Terminal::Draw() {
    static const GLfloat clearColor[4]{0.2f, 0.2f, 0.2f, 1.0f};
    glClearBufferfv(GL_COLOR, 0, clearColor);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const float textHeight = m_textRenderer.FontSize().y;

    char buffer[LOG_MAX_CHARS];
    sprintf_s(buffer, "fps = %.2f, size = %.0f x %.0f", m_fps, m_size.x, m_size.y);
    m_textRenderer.DrawText(buffer, {0, m_size.y - textHeight}, {1, 1, 1, 1});

    m_textRenderer.DrawText({m_input.data(), m_input.size()}, {0, 0}, {1, 0.5, 0, 1});

    IterateLatestLogs([this, textHeight](int i, const LogEntry &log) {
        static constexpr Vec4 colors[5]{
                {0.5f, 1.0f, 0.5f, 1.0f},
                {0.5f, 1.0f, 1.0f, 1.0f},
                {1.0f, 1.0f, 1.0f, 1.0f},
                {1.0f, 1.0f, 0.5f, 1.0f},
                {1.0f, 0.5f, 0.5f, 1.0f},
        };

        const Vec4 &color = colors[static_cast<int>(log.level)];
        m_textRenderer.DrawText(log.message, {0, static_cast<float>(i + 1) * textHeight}, color);
    });

    glBindVertexArray(0);
    glBindTextureUnit(0, 0);
    glUseProgram(0);

    glBlendFunc(GL_ONE, GL_ZERO);
    glDisable(GL_BLEND);
}
