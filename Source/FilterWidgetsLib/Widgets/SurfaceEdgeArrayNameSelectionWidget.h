#ifndef _SurfaceEdgeArrayNameSelectionWidget_H_
#define _SurfaceEdgeArrayNameSelectionWidget_H_




#include <QtCore/QObject>
#include <QtGui/QWidget>


#include "DREAM3DLib/Common/AbstractFilter.h"

#include "FilterWidgetsLib/QFilterParameterWidget.h"
#include "FilterWidgetsLib/FilterWidgetsLib.h"

#include "FilterWidgetsLib/ui_SurfaceEdgeArrayNameSelectionWidget.h"


/**
* @brief
* @author
* @version
*/
class FilterWidgetsLib_EXPORT SurfaceEdgeArrayNameSelectionWidget : public QWidget, private Ui::SurfaceEdgeArrayNameSelectionWidget
{
  Q_OBJECT

  public:
    /**
    * @brief Constructor
    * @param parameter The FilterParameter object that this widget represents
    * @param filter The instance of the filter that this parameter is a part of
    * @param parent The parent QWidget for this Widget
    */
    SurfaceEdgeArrayNameSelectionWidget(FilterParameter* parameter, AbstractFilter* filter = NULL, QWidget* parent = NULL);
    
    virtual ~SurfaceEdgeArrayNameSelectionWidget();
    
    /**
    * @brief This method does additional GUI widget connections
    */
    void setupGui();

  public slots:


  private:
    AbstractFilter*   m_Filter;
    FilterParameter*  m_FilterParameter;

    SurfaceEdgeArrayNameSelectionWidget(const SurfaceEdgeArrayNameSelectionWidget&); // Copy Constructor Not Implemented
    void operator=(const SurfaceEdgeArrayNameSelectionWidget&); // Operator '=' Not Implemented

};

#endif /* _SurfaceEdgeArrayNameSelectionWidget_H_ */

