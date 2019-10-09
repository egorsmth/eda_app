QT += charts
requires(qtConfig(combobox))

HEADERS += \
    analysis.h \
    model.h \
    point.h \
    themewidget.h \
    transform.h

SOURCES += \
    analysis.cpp \
    main.cpp \
    model.cpp \
    themewidget.cpp \
    transform.cpp

target.path = $$[QT_INSTALL_EXAMPLES]/charts/chartthemes
INSTALLS += target

FORMS += \
    themewidget.ui
