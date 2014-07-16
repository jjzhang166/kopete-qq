project ( kopete-qq )

add_definitions( -DKDE_DEFAULT_DEBUG_AREA=14210 )
set(CMAKE_CXX_FLAGS "-fpermissive")

add_subdirectory( icons ) 

include_directories( 
${KOPETE_INCLUDES} 
${CMAKE_CURRENT_SOURCE_DIR}/ui/ 
${LWQQ_INCLUDE_DIRS}
)

########### next target ###############

set(kopete_qq_PART_SRCS 
   qqprotocol.cpp 
   qq_types.cpp
   qqcontact.cpp 
   qqaccount.cpp 
   qqaddcontactpage.cpp 
   qqeditaccountwidget.cpp 
   qqbridgecallback.cpp
   qqloginverifywidget.cpp
   qquserinfoform.cpp
   qqchatsession.cpp
   qqdiscuchatsession.cpp
   qqgroupchatsession.cpp
   qqshowgetinfo.cpp
   translate.cpp
   trex.cpp
   ui/qqwebcamdialog.cpp 
)



kde4_add_ui_files(kopete_qq_PART_SRCS
qqaddui.ui
)

kde4_add_ui_files(kopete_qq_PART_SRCS
qqaccountpreferences.ui
)

kde4_add_ui_files(kopete_qq_PART_SRCS
qquserinfoform.ui
)

kde4_add_plugin(kopete_qq ${kopete_qq_PART_SRCS})

set(SHARE_DIR "${CMAKE_INSTALL_PREFIX}/share")
set(datadir ${SHARE_DIR}/lwqq)
configure_file(${PROJECT_SOURCE_DIR}/config.h.in ${PROJECT_BINARY_DIR}/config.h)
target_link_libraries(kopete_qq ${KDE4_KIO_LIBS} kopete  lwqq ${LIBJS_LIBRARIES})

 
install(TARGETS kopete_qq  DESTINATION ${PLUGIN_INSTALL_DIR})

########### install files ###############

install( FILES kopete_qq.desktop  DESTINATION ${SERVICES_INSTALL_DIR})
install( FILES qqdiscuui.rc qqchatui.rc qqgroupui.rc  DESTINATION ${DATA_INSTALL_DIR}/kopete_qq)
