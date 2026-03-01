pragma Singleton
import QtQuick

/**
 * Theme — Centralised design tokens with dark / light mode support.
 *
 * Toggle `darkMode` to switch the entire palette.  Every color property
 * is a simple ternary so that all bindings update automatically.
 */
QtObject {
    // -- Dark / Light toggle --
    property bool darkMode: false

    // -- Spacing (8px base grid) --
    readonly property int unit: 8
    readonly property int outerMargin:    unit * 3       // 24 — window-edge inset
    readonly property int sectionSpacing: unit * 2.5     // 20 — gap between major sections
    readonly property int contentSpacing: unit * 2       // 16 — inside-section element gap
    readonly property int itemSpacing:    unit           // 8  — tight row / column gap

    // -- Typography (pixel sizes) --
    readonly property int fontPageTitle:    22
    readonly property int fontSectionTitle: 15
    readonly property int fontTableHeader:  13
    readonly property int fontBody:         13
    readonly property int fontMeta:         12

    // -- Color Palette (adapts to darkMode) --
    readonly property color bgApp:        darkMode ? "#0F172A" : "#F1F5F9"
    readonly property color bgCard:       darkMode ? "#1E293B" : "#FFFFFF"
    readonly property color border:       darkMode ? "#334155" : "#E2E8F0"

    readonly property color textPrimary:  darkMode ? "#F1F5F9" : "#0F172A"
    readonly property color textSecondary:darkMode ? "#94A3B8" : "#64748B"

    readonly property color actionMain:   darkMode ? "#E2E8F0" : "#0F172A"
    readonly property color actionHover:  darkMode ? "#CBD5E1" : "#334155"
    readonly property color actionMuted:  darkMode ? "#1E293B" : "#F1F5F9"
    readonly property color actionMutedH: darkMode ? "#334155" : "#E2E8F0"

    // -- Button text on actionMain background --
    readonly property color btnTextOnAction: darkMode ? "#0F172A" : "#FFFFFF"

    // -- Switch / Toggle --
    readonly property color switchTrackOn:  "#3B82F6"                          // Blue 500 — accent
    readonly property color switchTrackOff: darkMode ? "#475569" : "#CBD5E1"   // visible in both modes
    readonly property color switchHandle:   darkMode ? "#F1F5F9" : "#FFFFFF"   // white knob

    // -- Radii --
    readonly property int radiusCard:    16
    readonly property int radiusControl: 8
    readonly property int radiusPill:    20

    // -- Robot identity colours (vibrant, modern hexes) --
    readonly property var robotColors: ["#3B82F6", "#10B981", "#F59E0B"]
    readonly property var robotColorsLight: darkMode
        ? ["#1E3A5F", "#1A3D2E", "#3D2E0A"]
        : ["#EFF6FF", "#ECFDF5", "#FFFBEB"]
}