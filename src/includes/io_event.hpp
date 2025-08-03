#pragma once

namespace Core
{
    class IOEvent
    {
    private:
        /* data */
    public:
        // Enum to define the type of mouse event
        enum class Type
        {
            None,
            Moved,
            Scrolled,
            Entered, // Cursor entered the window client area
            Left,    // Cursor left the window client area
            Key
        };

        enum class Button
        {
            Unknown,
            Left,
            Right,
            Middle,
            X1,
            X2
        };

        enum class Action
        {
            Unknown,
            Press,
            Release,
            Repeat
        };

        enum class Modifier
        {
            Unknown,
            Shift,
            Control,
            Alt,
            Super
        };

        enum class Key
        {
            Unknown = -1,
            Space = 32,
            Apostrophe = 39,
            Comma = 44,
            Minus = 45,
            Period = 46,
            Slash = 47,
            Zero = 48,
            One = 49,
            Two = 50,
            Three = 51,
            Four = 52,
            Five = 53,
            Six = 54,
            Seven = 55,
            Eight = 56,
            Nine = 57,
            Semicolon = 59,
            Equal = 61,
            A = 65,
            B = 66,
            C = 67,
            D = 68,
            E = 69,
            F = 70,
            G = 71,
            H = 72,
            I = 73,
            J = 74,
            K = 75,
            L = 76,
            M = 77,
            N = 78,
            O = 79,
            P = 80,
            Q = 81,
            R = 82,
            S = 83,
            T = 84,
            U = 85,
            V = 86,
            W = 87,
            X = 88,
            Y = 89,
            Z = 90,
            LeftBracket = 91,
            Backslash = 92,
            RightBracket = 93,
            GraveAccent = 96
        };

        // Offset/Position data (relevant for most events)
        double x{};
        double y{};

        int scrollOffsetX{};
        int scrollOffsetY{};

        Button button{Button::Unknown};
        Action action{Action::Unknown};
        // 'mods' indicates which modifier keys (Shift, Ctrl, Alt, Super) were held down
        Modifier mods{Modifier::Unknown};
        int key{};
        int scancode{};

        Type type{Type::None};

        // ---------- Dragging ---------------
        bool isDragging{false};
        bool isDragStart{false};
        bool isDragDown{false};
        // bool isDragUpdate{false};
        bool isDragEnd{false};
        int px{0};
        int py{0};
        int dx{0};
        int dy{0};
        // ------------------------------------

        IOEvent(/* args */) = default;
        ~IOEvent() = default;

        void reset();
        void setMouseWheelEvent(double x, double y);
        void setMouseMoveEvent(double x, double y);
        void setMouseEnterExitEvent(bool entered);
        void setMouseButtonEvent(Button button, Action action, Modifier mods);
        void setKeyboardEvent(int key, int scancode, Action action, Modifier mods);

        bool isKey(int key, Key eKey) { return key == static_cast<int>(eKey); }
    };

} // namespace Core
