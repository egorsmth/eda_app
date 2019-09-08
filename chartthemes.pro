QT += charts
requires(qtConfig(combobox))

HEADERS += \
    model.h \
    themewidget.h

SOURCES += \
    main.cpp \
    model.cpp \
    themewidget.cpp

target.path = $$[QT_INSTALL_EXAMPLES]/charts/chartthemes
INSTALLS += target

FORMS += \
    themewidget.ui
