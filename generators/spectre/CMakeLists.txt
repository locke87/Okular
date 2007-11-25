add_subdirectory( libspectre conf ) 

include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}/../..
   ${CMAKE_CURRENT_BINARY_DIR}/../..
)

########### next target ###############

set(okularGenerator_ghostview_SRCS
   generator_ghostview.cpp
   rendererthread.cpp
)

kde4_add_ui_files(okularGenerator_ghostview_SRCS
   conf/gssettingswidget.ui
)

kde4_add_kcfg_files(okularGenerator_ghostview_SRCS conf/gssettings.kcfgc )


kde4_add_plugin(okularGenerator_ghostview WITH_PREFIX ${okularGenerator_ghostview_SRCS})

target_link_libraries(okularGenerator_ghostview  ${KDE4_KIO_LIBS} okularcore spectreOkular ${KDE4_KDEUI_LIBRARY} ${KDE4_KDE3SUPPORT_LIBRARY} )

install(TARGETS okularGenerator_ghostview DESTINATION ${PLUGIN_INSTALL_DIR})


########### install files ###############

install( FILES libokularGenerator_ghostview.desktop okularGhostview.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} )
install( FILES okularApplication_ghostview.desktop  DESTINATION  ${XDG_APPS_INSTALL_DIR} )
