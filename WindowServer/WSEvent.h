#pragma once

#include <Widgets/Point.h>
#include <Widgets/Rect.h>
#include <AK/AKString.h>
#include <AK/Types.h>

static const char* WSEvent_names[] = {
    "Invalid",
    "Show",
    "Hide",
    "Paint",
    "MouseMove",
    "MouseDown",
    "MouseUp",
    "KeyDown",
    "KeyUp",
    "Timer",
    "WM_Compose",
    "WM_Invalidate",
};

class WSEvent {
public:
    enum Type {
        Invalid = 0,
        Show,
        Hide,
        Paint,
        MouseMove,
        MouseDown,
        MouseUp,
        KeyDown,
        KeyUp,
        Timer,
        WM_Compose,
        WM_Invalidate,
    };

    WSEvent() { }
    explicit WSEvent(Type type) : m_type(type) { }
    virtual ~WSEvent() { }

    Type type() const { return m_type; }

    const char* name() const { return WSEvent_names[(unsigned)m_type]; }

    bool isMouseEvent() const { return m_type == MouseMove || m_type == MouseDown || m_type == MouseUp; }
    bool isKeyEvent() const { return m_type == KeyUp || m_type == KeyDown; }
    bool isPaintEvent() const { return m_type == Paint; }

private:
    Type m_type { Invalid };
};

class PaintEvent final : public WSEvent {
public:
    explicit PaintEvent(const Rect& rect = Rect())
        : WSEvent(WSEvent::Paint)
        , m_rect(rect)
    {
    }

    const Rect& rect() const { return m_rect; }
private:
    friend class WSWindowManager;
    Rect m_rect;
};

class ShowEvent final : public WSEvent {
public:
    ShowEvent()
        : WSEvent(WSEvent::Show)
    {
    }
};

class HideEvent final : public WSEvent {
public:
    HideEvent()
        : WSEvent(WSEvent::Hide)
    {
    }
};

enum class MouseButton : byte {
    None = 0,
    Left,
    Middle,
    Right,
};

enum KeyboardKey {
    Invalid,
    LeftArrow,
    RightArrow,
    UpArrow,
    DownArrow,
    Backspace,
    Return,
};

class KeyEvent final : public WSEvent {
public:
    KeyEvent(Type type, int key)
        : WSEvent(type)
        , m_key(key)
    {
    }

    int key() const { return m_key; }
    bool ctrl() const { return m_ctrl; }
    bool alt() const { return m_alt; }
    bool shift() const { return m_shift; }
    String text() const { return m_text; }

private:
    friend class WSEventLoop;
    friend class WSScreen;
    int m_key { 0 };
    bool m_ctrl { false };
    bool m_alt { false };
    bool m_shift { false };
    String m_text;
};

class MouseEvent final : public WSEvent {
public:
    MouseEvent(Type type, int x, int y, MouseButton button = MouseButton::None)
        : WSEvent(type)
        , m_position(x, y)
        , m_button(button)
    {
    }

    Point position() const { return m_position; }
    int x() const { return m_position.x(); }
    int y() const { return m_position.y(); }
    MouseButton button() const { return m_button; }

private:
    Point m_position;
    MouseButton m_button { MouseButton::None };
};

class TimerEvent final : public WSEvent {
public:
    TimerEvent() : WSEvent(WSEvent::Timer) { }
    ~TimerEvent() { }
};

