include(CMakePackageConfigHelpers)

install(
  TARGETS       ${AubergineName}
  EXPORT        ${AubergineName}Targets
  LIBRARY       DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  ARCHIVE       DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  RUNTIME       DESTINATION "${CMAKE_INSTALL_BINDIR}"
  INCLUDES      DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
  PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${AubergineName}")

install(
  EXPORT      ${AubergineName}Targets
  FILE        ${AubergineName}Targets.cmake
  NAMESPACE   ${AubergineName}::
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${AubergineName}")

configure_package_config_file(
  "${CMAKE_CURRENT_LIST_DIR}/${AubergineName}Config.template"
  "${CMAKE_CURRENT_BINARY_DIR}/${AubergineName}Config.cmake"
  INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${AubergineName}")

write_basic_package_version_file(
  "${AubergineName}ConfigVersion.cmake"
  VERSION "${AubergineVersion}"
  COMPATIBILITY AnyNewerVersion)

install(
  FILES       "${CMAKE_CURRENT_BINARY_DIR}/${AubergineName}Config.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/${AubergineName}ConfigVersion.cmake"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${AubergineName}")
