

import QtQuick  // viser graf

Rectangle {
    id: chart

    property var stockDataModel: null

    color: "white"
    radius: 8

    Canvas {
        id: canvas

        anchors.fill: parent
        anchors.margins: 12

        onPaint: {
            let ctx = getContext("2d")
            ctx.reset()

            if (!chart.stockDataModel ||
                chart.stockDataModel.count() < 2)
                return

            let count = chart.stockDataModel.count()
            let prices = []

            for (let i = 0; i < count; i++) {
                prices.push(
                    chart.stockDataModel.closeAt(i)
                )
            }

            let minPrice = Math.min(...prices)
            let maxPrice = Math.max(...prices)

            // Litt ekstra plass over og under linjen
            let padding = Math.max(
                (maxPrice - minPrice) * 0.15, 1
            )

            minPrice -= padding
            maxPrice += padding

            let left = 65
            let right = width - 20
            let top = 20
            let bottom = height - 40

            if (right <= left || bottom <= top)
                return

            // Tegn Y-akse og hjelpelinjer
            ctx.font = "12px sans-serif"
            ctx.textAlign = "right"
            ctx.textBaseline = "middle"

            for (let i = 0; i <= 4; i++) {
                let price = minPrice +
                    (maxPrice - minPrice) * i / 4

                let y = bottom -
                    i * (bottom - top) / 4

                ctx.beginPath()
                ctx.strokeStyle = "#e2e8f0"
                ctx.lineWidth = 1
                ctx.moveTo(left, y)
                ctx.lineTo(right, y)
                ctx.stroke()

                ctx.fillStyle = "#475569"
                ctx.fillText(
                    price.toFixed(1),
                    left - 10, y
                )
            }

            // Tegn X-akse med datoer
            ctx.textAlign = "center"
            ctx.textBaseline = "top"
            ctx.fillStyle = "#475569"

            let dateStep = Math.max(
                1, Math.ceil(count / 5)
            )

            for (let i = 0; i < count; i += dateStep) {
                let x = left +
                    i * (right - left) / (count - 1)

                ctx.fillText(
                    chart.stockDataModel.dateAt(i).slice(5),
                    x, bottom + 10
                )
            }

            // Tegn kurslinjen
            ctx.beginPath()
            ctx.strokeStyle = "#2563eb"
            ctx.lineWidth = 3

            for (let i = 0; i < count; i++) {
                let x = left +
                    i * (right - left) / (count - 1)

                let y = bottom -
                    (prices[i] - minPrice) /
                    (maxPrice - minPrice) *
                    (bottom - top)

                if (i === 0)
                    ctx.moveTo(x, y)
                else
                    ctx.lineTo(x, y)
            }

            ctx.stroke()

            // Tegn punktene
            ctx.fillStyle = "#2563eb"

            for (let i = 0; i < count; i++) {
                let x = left +
                    i * (right - left) / (count - 1)

                let y = bottom -
                    (prices[i] - minPrice) /
                    (maxPrice - minPrice) *
                    (bottom - top)

                ctx.beginPath()
                ctx.arc(x, y, 4, 0, Math.PI * 2)
                ctx.fill()
            }
        }

        onWidthChanged: requestPaint()
        onHeightChanged: requestPaint()
    }

    Connections {
        target: chart.stockDataModel
        ignoreUnknownSignals: true

        function onModelReset() {
            canvas.requestPaint()
        }
    }

    onStockDataModelChanged: canvas.requestPaint()
}
