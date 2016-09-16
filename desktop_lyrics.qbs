import qbs

CppApplication {
    name: "Desktop Lyrics"
    Depends { name: "Qt"; submodules: [ "core", "quick", "quickcontrols2", "gui", "dbus" ] }

    cpp.cxxLanguageVersion: "c++14"
    cpp.enableExceptions: true
    consoleApplication: false

    files: [
        "desktop_lyrics.cpp",
        "desktop_lyrics.h",
        "lyrics_parser.cpp",
        "lyrics_parser.h",
        "lyrics_window.cpp",
        "lyrics_window.h",
        "main.cpp",
        "media_service.cpp",
        "media_service.h",
        "file_search.h",
        "file_search.cpp",
    ]

    Group {
        fileTagsFilter: product.type
        qbs.install: true
    }

    Group {
        files: [
            "ui/CompactViewer.qml",
            "ui/LyricsViewer.qml",
            "ui/LyricsViewerUI.ui.qml",
            "ui/main.qml",
        ]
        name: "QML Files"
        qbs.install: true
        qbs.installDir: "ui/"
    }
}
