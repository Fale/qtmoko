qtopia_project(qtopia plugin)
TARGET=htcuniversalvendor

CONFIG+=no_tr

HEADERS		=  htcuniversalgsmpower.h htcuniversalrffunctionality.h htcuniversalmodemnetworkregistration.h htcuniversalcallprovider.h htcuniversalbandselection.h htcuniversalcallvolume.h htcuniversalmodemservice.h htcuniversalpinmanager.h htcuniversalplugin.h htcuniversalpreferrednetworkoperators.h htcuniversalsimtoolkit.h htcuniversalvibrateaccessory.h htcuniversalplugin.h
SOURCES	        =  htcuniversalgsmpower.cpp htcuniversalrffunctionality.cpp htcuniversalmodemnetworkregistration.cpp htcuniversalcallprovider.cpp htcuniversalpreferrednetworkoperators.cpp htcuniversalcallvolume.cpp htcuniversalvibrateaccessory.cpp htcuniversalmodemservice.cpp htcuniversalbandselection.cpp htcuniversalpinmanager.cpp htcuniversalsimtoolkit.cpp htcuniversalplugin.cpp

depends(libraries/qtopiaphonemodem)