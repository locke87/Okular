/*
 *   Copyright 2012 Marco Martin <mart@kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as
 *   published by the Free Software Foundation; either version 2,
 *   or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

import QtQuick 2.1
import org.kde.okular 2.0 as Okular
import QtQuick.Controls 1.3
import org.kde.plasma.extras 2.0 as PlasmaExtras
import org.kde.plasma.components 2.0 as PlasmaComponents

ApplicationWindow {
    id: fileBrowserRoot
    objectName: "fileBrowserRoot"
    visible: true

    width: 360
    height: 360

    /*TODO: port ResourceInstance
    PlasmaExtras.ResourceInstance {
        id: resourceInstance
        uri: documentItem.path
    }*/

    Okular.DocumentItem {
        id: documentItem
        onWindowTitleForDocumentChanged: {
            fileBrowserRoot.title = windowTitleForDocument
        }
    }

    PlasmaComponents.PageStack {
        id: mainStack
        clip: false
        anchors.fill: parent
    }

    //FIXME: this is due to global vars being binded after the parse is done, do the 2 steps parsing
    Timer {
        interval: 100
        running: true
        onTriggered: {
            if (commandlineArguments.length > 0) {
                documentItem.path = commandlineArguments[0]
            }

            var browser = mainStack.push(Qt.createComponent("Browser.qml"))
            if (commandlineArguments.length == 0) {
                browser.open = true;
            }
        }
    }
}
