
add_rules("mode.debug", "mode.release")

target("WaveDisplaySystem")
    add_rules("qt.widgetapp")
    add_headerfiles("src/*.h")
    add_files("src/*.cpp")
    add_files("src/*.ui")

    -- qt moc
    add_files("src/homewindow.h")
    add_files("src/mainwindow.h")
    add_files("src/monitorwindow.h")
    add_files("src/settingwindow.h")
    add_files("src/scatterwindow.h")

    add_files("res.qrc")

    add_frameworks("QtCore", "QtWidgets", "QtGui", "QtCharts", "QtSerialPort")
