include(GNUInstallDirs)

install(
  TARGETS       ${AubergineName}
  LIBRARY       DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${AubergineName}")
