
set(DREAM3DWidgetsLib_ParameterWidgets_HDRS "")
set(DREAM3DWidgetsLib_ParameterWidgets_SRCS "")
set(DREAM3DWidgetsLib_ParameterWidgets_UIS "")


set(DREAM3D_PARAMETER_WIDGETS
      StringWidget
      IntWidget
      DoubleWidget
      InputFileWidget
      InputPathWidget
      OutputFileWidget
      OutputPathWidget
      BooleanWidget
      ChoiceWidget
      IntVec3Widget
      FloatVec3Widget
      AxisAngleWidget
      DataArraySelectionWidget
      DataContainerArrayProxyWidget
      ComparisonSelectionWidget
      AttributeMatrixSelectionWidget
      DataContainerSelectionWidget
      ShapeTypeSelectionWidget
      PreflightUpdatedValueWidget
#      CellArrayComparisonSelectionWidget
#      FeatureArrayComparisonSelectionWidget
#      EnsembleArrayComparisonSelectionWidget
#      VertexArrayComparisonSelectionWidget
#      FaceArrayComparisonSelectionWidget
#      EdgeArrayComparisonSelectionWidget
      UnknownWidget
)

set(REGISTER_KNOWN_WIDGETS ${DREAM3DWidgetsLib_BINARY_DIR}/FilterWidgetManager_RegisterWidgets.cpp)
set(FILTER_WIDGET_HEADERS ${DREAM3DWidgetsLib_BINARY_DIR}/FilterWidgetHeaders.h)
file(WRITE ${REGISTER_KNOWN_WIDGETS}  "/* This file is automatically generated. DO NOT EDIT this file. It will be over written */\n")
file(WRITE ${FILTER_WIDGET_HEADERS}  "/* This file is automatically generated. DO NOT EDIT this file. It will be over written */\n")

foreach(FPW ${DREAM3D_PARAMETER_WIDGETS})
  set(DREAM3DWidgetsLib_ParameterWidgets_HDRS ${DREAM3DWidgetsLib_ParameterWidgets_HDRS}
    ${DREAM3DWidgetsLib_SOURCE_DIR}/FilterParameterWidgets/${FPW}.h
    )
  set(DREAM3DWidgetsLib_ParameterWidgets_SRCS ${DREAM3DWidgetsLib_ParameterWidgets_SRCS}
    ${DREAM3DWidgetsLib_SOURCE_DIR}/FilterParameterWidgets/${FPW}.cpp
    )
  set(DREAM3DWidgetsLib_ParameterWidgets_UIS ${DREAM3DWidgetsLib_ParameterWidgets_UIS}
    ${DREAM3DWidgetsLib_SOURCE_DIR}/FilterParameterWidgets/UI_Files/${FPW}.ui
    )

  file(APPEND  ${REGISTER_KNOWN_WIDGETS} "{\n")
  file(APPEND  ${REGISTER_KNOWN_WIDGETS} "  PipelineFilterWidgetFactory<${FPW}>::Pointer factory = PipelineFilterWidgetFactory<${FPW}>::New();\n")
  file(APPEND  ${REGISTER_KNOWN_WIDGETS} "  idManager->addFilterWidgetFactory( \"${FPW}\", factory );\n")
  file(APPEND  ${REGISTER_KNOWN_WIDGETS} "}\n")
  file(APPEND  ${FILTER_WIDGET_HEADERS} "#include \"DREAM3DWidgetsLib/FilterParameterWidgets/${FPW}.h\"\n")
endforeach()

# --------------------------------------------------------------------
# Add some support files that still need MOC and UIC to be run on them
set(DREAM3DWidgetsLib_ParameterWidgets_HDRS
  ${DREAM3DWidgetsLib_ParameterWidgets_HDRS}
  ${DREAM3DWidgetsLib_SOURCE_DIR}/FilterParameterWidgets/ComparisonSelectionTableModel.h
  ${DREAM3DWidgetsLib_SOURCE_DIR}/FilterParameterWidgets/ComparisonSelectionItemDelegate.h
  ${DREAM3DWidgetsLib_SOURCE_DIR}/FilterParameterWidgets/FilterParameterWidgetsDialogs.h
)
set(DREAM3DWidgetsLib_ParameterWidgets_SRCS
  ${DREAM3DWidgetsLib_ParameterWidgets_SRCS}
  ${DREAM3DWidgetsLib_SOURCE_DIR}/FilterParameterWidgets/ComparisonSelectionTableModel.cpp
  ${DREAM3DWidgetsLib_SOURCE_DIR}/FilterParameterWidgets/ComparisonSelectionItemDelegate.cpp
  ${DREAM3DWidgetsLib_SOURCE_DIR}/FilterParameterWidgets/FilterParameterWidgetsDialogs.cpp
)
set(DREAM3DWidgetsLib_ParameterWidgets_UIS
  ${DREAM3DWidgetsLib_ParameterWidgets_UIS}
  )




cmp_IDE_SOURCE_PROPERTIES( "DREAM3DWidgetsLib/FilterParameterWidgets" "${DREAM3DWidgetsLib_ParameterWidgets_HDRS}" "${DREAM3DWidgetsLib_ParameterWidgets_SRCS}" "0")

cmp_IDE_GENERATED_PROPERTIES("DREAM3DWidgetsLib/FilterParameterWidgets/UI_Files" "${DREAM3DWidgetsLib_ParameterWidgets_UIS}" "")

# --------------------------------------------------------------------
# and finally this will run moc:
#QT4_WRAP_CPP( DREAM3DWidgetsLib_ParameterWidgets_Generated_MOC_SRCS ${DREAM3DWidgetsLib_ParameterWidgets_HDRS} )

# These generated moc files will be #include in the FilterWidget source file that
# are generated so we need to tell the build system to NOT compile these files
set_source_files_properties( ${DREAM3DWidgetsLib_ParameterWidgets_Generated_MOC_SRCS} PROPERTIES HEADER_FILE_ONLY TRUE)

# -- Run MOC and UIC on the necessary files
# QT4_ADD_RESOURCES( DREAM3DWidgetsLib_Generated_RC_SRCS "${DREAM3DProj_SOURCE_DIR}/Documentation/Filters/Generated_FilterDocs.qrc"  )

# --------------------------------------------------------------------
# Continue on with our Qt4 section
QT4_WRAP_UI( DREAM3DWidgetsLib_ParameterWidgets_Generated_UI_HDRS ${DREAM3DWidgetsLib_ParameterWidgets_UIS} )

# --------------------------------------------------------------------
#-- Put the Qt generated files into their own group for IDEs
cmp_IDE_SOURCE_PROPERTIES( "Generated/Qt_Moc" "" "${DREAM3DWidgetsLib_ParameterWidgets_Generated_MOC_SRCS}" "0")
cmp_IDE_SOURCE_PROPERTIES( "Generated/Qt_Uic" "${DREAM3DWidgetsLib_ParameterWidgets_Generated_UI_HDRS}" "" "0")
#cmp_IDE_SOURCE_PROPERTIES( "Generated/Qt_Qrc" "${DREAM3DWidgetsLib_Generated_RC_SRCS}" "" "0")




