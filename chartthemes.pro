QT += charts
requires(qtConfig(combobox))

HEADERS += \
    analysis.h \
    audio.h \
    callout.h \
    model.h \
    point.h \
    themewidget.h \
    transform.h

SOURCES += \
    analysis.cpp \
    callout.cpp \
    main.cpp \
    model.cpp \
    themewidget.cpp \
    transform.cpp

target.path = $$[QT_INSTALL_EXAMPLES]/charts/chartthemes
INSTALLS += target

FORMS += \
    themewidget.ui

DISTFILES += \
    res.txt
