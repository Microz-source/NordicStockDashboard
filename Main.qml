
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: window

    width: 900
    height: 600
    visible: true

    title: "Nordic Stock Dashboard"

    color: "#f5f7fa"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 16

        Label {
            text: "Nordic Stock Dashboard"
            font.pixelSize: 28
            font.bold: true
            color: "#1e293b"
        }


        RowLayout {
            Layout.fillWidth: true
            spacing: 16

            Label {
                text: "Velg aksje:"
                font.pixelSize: 16
                color: "#475569"
            }


            ComboBox {      //aksjevelger
                id: stockSelector

                Layout.preferredWidth: 280

                model: [
                    "Mowi ASA",
                    "SpareBank 1 SMN",
                    "Yara International ASA"
                ]

                currentIndex: 0

                contentItem: Text {
                    leftPadding: 10
                    rightPadding: 30

                    text: stockSelector.displayText
                    color: "#1e293b"
                    font.pixelSize: 14

                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                onActivated: function(index) {
                    const symbols = [
                        "MOWI.OL",
                        "MING.OL",
                        "YAR.OL"
                    ]

                    var periods = [7, 30, 90]

                    stockApiClient.fetchStockData(
                        symbols[index],
                        periods[periodSelector.currentIndex]
                    )
                }
            }
            Label {
                text: "Tidsperiode:"
                color: "#1e293b"
            }

            ComboBox {          // tidsperiodevelger
                id: periodSelector
                Layout.preferredWidth: 130

                model: ["7 dager", "30 dager", "90 dager"]
                currentIndex: 1

                onActivated: function(index) {
                    var symbols = ["MOWI.OL", "MING.OL", "YAR.OL"]
                    var periods = [7, 30, 90]

                    stockApiClient.fetchStockData(
                        symbols[stockSelector.currentIndex],
                        periods[index]
                    )
                }

                contentItem: Text {
                    leftPadding: 10
                    rightPadding: 30
                    text: periodSelector.displayText
                    color: "#1e293b"
                    font.pixelSize: 14
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }
            }

            Button {            // oppdatering
                id: refreshButton
                text: "Oppdater"
                palette.buttonText: "#1e293b"

                onClicked: {
                    var symbols = ["MOWI.OL", "MING.OL", "YAR.OL"]
                    var periods = [7, 30, 90]

                    stockApiClient.fetchStockData(
                        symbols[stockSelector.currentIndex],
                        periods[periodSelector.currentIndex]
                    )
                }
            }

            Label {
                text: "Sortering:"
                color: "#1e293b"
            }

            ComboBox {
                id: sortSelector
                Layout.preferredWidth: 150

                model: ["Eldste først", "Nyeste først"]
                currentIndex: 0

                contentItem: Text {
                    leftPadding: 10
                    rightPadding: 30
                    text: sortSelector.displayText
                    color: "#1e293b"
                    font.pixelSize: 14
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }
            }
        }

        Label {
            text: "Historisk kursutvikling"
            font.pixelSize: 18
            font.bold: true
            color: "#1e293b"
        }

        StockChart {
            Layout.fillWidth: true
            Layout.preferredHeight: 220

            stockDataModel: stockModel
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            color: "#dbeafe"
            radius: 6

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 16
                anchors.rightMargin: 16


                Label {
                    text: "Dato"
                    Layout.fillWidth: true
                    font.bold: true
                }

                Label {
                    text: "Sluttkurs (NOK)"
                    font.bold: true
                }
            }
        }

        ListView {
            id: stockList

            Layout.fillWidth: true
            Layout.fillHeight: true

            clip: true
            spacing: 4

            model: stockModel
            property bool newestFirst: sortSelector.currentIndex === 1

            delegate: Rectangle {
                width: stockList.width
                height: 44
                radius: 5

                color: index % 2 === 0
                       ? "#ffffff" : "#edf2f7"

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 16
                    anchors.rightMargin: 16

                    Label {
                        text: stockModel.dateAt(
                            stockList.newestFirst
                                ? stockModel.count() - 1 - index
                                : index
                        )
                        Layout.fillWidth: true
                        color: "#334155"
                    }

                    Label {
                        text: Number(stockModel.closeAt(
                            stockList.newestFirst
                                ? stockModel.count() - 1 - index
                                : index
                        )).toFixed(2) + " kr"
                        color: "#334155"
                    }
                }
            }
        }
    }
    Connections {
        target: stockApiClient

        function onErrorOccurred(message) {
            errorDialog.text = message
            errorDialog.open()
        }
    }

    Dialog {
        id: errorDialog
        title: "Feil ved henting av aksjekurser"
        modal: true
        anchors.centerIn: parent
        standardButtons: Dialog.Ok

        property alias text: errorLabel.text

        contentItem: Label {
            id: errorLabel
            color: "#1e293b"
            wrapMode: Text.WordWrap
            width: 300
        }
    }
}

// ComboBox lager en nedtrekksliste
// onActivated kjøres når brukeren velger en aksje
// symbols[index] finner riktig aksjesymbol.
// fetchStockData() ber C++ hente nye data fra EODHD