import QtQuick 2.4
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Item {
    width: 800
    height: 600
    property alias sbOffset: sbOffset
    property alias topLayout: topLayout
    property alias controlLayout: controlLayout
    property alias bottomLayout: bottomLayout
    property alias searchView: searchView
    property alias txLyricsFolder: txLyricsFolder
    property alias lbGenre: lbGenre
    property alias lbAlbum: lbAlbum
    property alias lbArtist: lbArtist
    property alias lbTitle: lbTitle
    property alias mediaInfoR: mediaInfoR
    property alias btnPause: btnPause
    property alias mediaPosition: mediaPosition
    property alias mediaArt: mediaArt
    property alias btnPrev: btnPrev
    property alias btnPlay: btnPlay
    property alias btnStop: btnStop
    property alias btnNext: btnNext
    property alias btnOpenLyrics: btnOpenLyrics
    property alias lyricsArea: lyricsArea
    property alias tooltipMediaPosition: tooltipMediaPosition

    ColumnLayout {
        id: columnLayout1
        anchors.rightMargin: 10
        anchors.leftMargin: 10
        anchors.bottomMargin: 10
        anchors.topMargin: 10
        anchors.fill: parent


        GridLayout {
            id: topLayout
            Layout.maximumHeight: 100
            columns: 3
            Layout.preferredHeight: 100
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop


            Image {
                id: mediaArt
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillHeight: true
                sourceSize.width: 120
                fillMode: Image.PreserveAspectFit
                source: "/tmp/clementine-art-r18373.jpg"
            }
            GridLayout {
                id: mediaInfoLayout
                clip: true
                columns: 2
                Layout.fillHeight: true
                Layout.fillWidth: true

                Label {
                    id: lbTitle
                    text: qsTr("Title")
                    clip: true
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 15
                    font.bold: true
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }

                Label {
                    id: mediaInfoR
                    text: qsTr("")
                    Layout.fillWidth: false
                    Layout.fillHeight: true
                }

                Label {
                    id: lbArtist
                    text: qsTr("Artist")
                    Layout.fillHeight: true
                    clip: true
                    font.bold: true
                    font.pointSize: 12
                    Layout.fillWidth: true
                }

                Label {
                    id: label3
                    text: qsTr("")
                }

                Label {
                    id: lbAlbum
                    text: qsTr("Album")
                    clip: true
                    Layout.fillWidth: true
                }

                Label {
                    id: label5
                    text: qsTr("")
                }

                Label {
                    id: lbGenre
                    text: qsTr("Genre")
                    clip: true
                    Layout.fillWidth: true
                }

                Label {
                    id: label7
                    text: qsTr("")
                }
            }

            ColumnLayout {
                id: columnLayout2
                Layout.alignment: Qt.AlignLeft | Qt.AlignBottom
                spacing: 0
                Layout.fillHeight: true

                Label {
                    id: label2
                    text: qsTr("Offset")
                    font.pointSize: 6
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }

                SpinBox {
                    id: sbOffset
                    scale: 0.7
                    stepSize: 100
                    to: 100000
                    from: -100000
                }
            }
        }



        Label {
            id: lyricsArea
            text: qsTr("Lyrics")
            Layout.maximumHeight: 60
            Layout.fillHeight: true
            rightPadding: 10
            leftPadding: 10
            wrapMode: Text.WordWrap
            Layout.columnSpan: 3
            verticalAlignment: Text.AlignVCenter
            font.pointSize: 14
            horizontalAlignment: Text.AlignHCenter
            Layout.fillWidth: true
        }

        Slider {
            id: mediaPosition
            Layout.preferredHeight: 30
            Layout.fillWidth: true
            Layout.columnSpan: 3
            value: 0.5

            ToolTip {
                id: tooltipMediaPosition
                parent: mediaPosition.handle
                visible: mediaPosition.pressed
            }
        }

        RowLayout {
            id: controlLayout
            Layout.preferredHeight: 40
            Layout.fillWidth: true
            Layout.columnSpan: 3








            Item {
                id: item1
                Layout.fillWidth: true
            }
            Button {
                id: btnPrev
                font.family: "FontAwesome"
                text: qsTr("\uf048")
                hoverEnabled: true
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Previous")
            }
            Button {
                id: btnPlay
                font.family: "FontAwesome"
                text: qsTr("\uf04b")
                hoverEnabled: true
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Play")
            }
            Button {
                id: btnPause
                font.family: "FontAwesome"
                text: qsTr("\uf04c")
                hoverEnabled: true
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Pause")
            }
            Button {
                id: btnStop
                font.family: "FontAwesome"
                text: qsTr("\uf04d")
                hoverEnabled: true
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Stop")
            }
            Button {
                id: btnNext
                font.family: "FontAwesome"
                text: qsTr("\uf051")
                hoverEnabled: true
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Next")
            }
            Item {
                id: item2
                Layout.fillWidth: true
            }
        }
        ListView {
            id: searchView
            clip: true
            Layout.fillHeight: true
            Layout.fillWidth: true
            ScrollBar.vertical: ScrollBar{ }
        }
        RowLayout {
            id: bottomLayout
            width: 100
            height: 100

            Label {
                id: label1
                text: qsTr("Lyrics Folder")
            }

            TextField {
                id: txLyricsFolder
                text: qsTr("Text Field")
                Layout.fillWidth: true
            }

            Button {
                id: btnOpenLyrics
                text: qsTr("Browse Lyrics")
            }
        }
    }
}
