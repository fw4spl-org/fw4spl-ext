
set( NAME BeginnerTraining01 )
set( VERSION 0.1 )
set( TYPE APP )
set( DEPENDENCIES  )
set( REQUIREMENTS
    servicesReg
    dataReg
    gui
    guiQt
    fwlauncher
    appXml
    beginnerTraining
)

bundleParam(appXml PARAM_LIST config PARAM_VALUES BeginnerTraining01Config)
