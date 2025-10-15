import QtQuick 2.12
import QtQuick.Controls 2.12
import com.wearily.WeaChart 1.0

Row {
    id: root
    property int type // SeriesType
    property int shape // markerShape
    property string iconUrl // markerIconUrl
    property string name
    property color color: series.color
    property color color2: "white"
    property real mixFactor: 0.5

    property color labelColor
    property font labelFont

    spacing: 8

    function mix(c1, c2, factor) {
        return Qt.rgba(
            c1.r * (1-factor) + c2.r * factor,
            c1.g * (1-factor) + c2.g * factor,
            c1.b * (1-factor) + c2.b * factor,
            c1.a * (1-factor) + c2.a * factor)
    }

    // Marker
    Canvas {
        id: marker
        width: 24
        height: 16

        property var img: Qt.createQmlObject("import QtQuick 2.0; Image{visible: false}", marker)

        onPaint: {
            var ctx = getContext("2d")
            ctx.clearRect(0, 0, width, height)
            ctx.strokestyle = ctx.fillStyle = mix(root.color, root.color2, root.mixFactor)

            if (type === GL.SeriesTypeLine) {
                ctx.lineWidth = 2
                ctx.beginPath()
                ctx.moveTo(0, height / 2)
                ctx.lineTo(width, height / 2)
                ctx.strokeStyle = ctx.fillStyle = mix(root.color, root.color2, root.mixFactor)
                ctx.stroke()
            }
            else if (type === GL.SeriesTypeScatter) {
                if (shape === GL.ShapeTexture) {
                    img.source = "qrc" + iconUrl
                    if (img.status === Image.Ready) {
                        ctx.drawImage(img, 0, 0, width, height)
                    }

                }
                else {
                    ctx.beginPath()
                    ctx.arc(width/2, height/2, 5, 0, Math.PI*2)
                    ctx.fill()
                }

            }
            else if (type === GL.SeriesTypeArea) {
                ctx.fillRect(0, 0, width, height)
            }
        }
    }
    // Label
    Text {
        text: name
        color: labelColor
        font: labelFont
        verticalAlignment: Text.AlignVCenter
    }

}
