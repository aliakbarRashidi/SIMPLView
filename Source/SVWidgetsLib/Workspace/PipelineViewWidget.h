/* ============================================================================
* Copyright (c) 2009-2016 BlueQuartz Software, LLC
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*
* Redistributions of source code must retain the above copyright notice, this
* list of conditions and the following disclaimer.
*
* Redistributions in binary form must reproduce the above copyright notice, this
* list of conditions and the following disclaimer in the documentation and/or
* other materials provided with the distribution.
*
* Neither the name of BlueQuartz Software, the US Air Force, nor the names of its
* contributors may be used to endorse or promote products derived from this software
* without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
* USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
* The code contained herein was partially funded by the followig contracts:
*    United States Air Force Prime Contract FA8650-07-D-5800
*    United States Air Force Prime Contract FA8650-10-D-5210
*    United States Prime Contract Navy N00173-07-C-2068
*
* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */


#ifndef _pipelineviewwidget_h_
#define _pipelineviewwidget_h_

#include <vector>

#include <QtCore/QTimer>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QStatusBar>
#include <QtGui/QPainter>


#include "SIMPLib/Common/PipelineMessage.h"
#include "SIMPLib/Common/FilterPipeline.h"
#include "SIMPLib/FilterParameters/H5FilterParametersReader.h"
#include "SIMPLib/FilterParameters/H5FilterParametersWriter.h"
#include "SIMPLib/CoreFilters/DataContainerReader.h"

#include "SVWidgetsLib/SVWidgetsLib.h"
#include "SVWidgetsLib/Workspace/PipelineFilterWidget.h"
#include "SVWidgetsLib/Widgets/DropBoxWidget.h"

#include "Applications/SIMPLView/SIMPLViewApplication.h"

#include "SVWidgetsLib/QtSupport/QtSFileDragMessageBox.h"

class QScrollArea;
class QContextMenuEvent;
class QLabel;
class QEvent;

/*
 *
 */
class SVWidgetsLib_EXPORT PipelineViewWidget : public QFrame
{
    Q_OBJECT

  public:
    PipelineViewWidget(QWidget* parent = 0);
    virtual ~PipelineViewWidget();

    /**
     * @brief filterCount
     * @return
     */
    int filterCount();

    /**
     * @brief filterWidgetAt
     * @param index
     * @return
     */
    PipelineFilterWidget* filterWidgetAt(int index);

    /**
    * @brief containsFilterWidget
    * @param filterWidget
    * @return
    */
    bool containsFilterWidget(PipelineFilterWidget* filterWidget);

    /**
    * @brief indexOfFilterWidget
    * @param filterWidget
    * @return
    */
    int indexOfFilterWidget(PipelineFilterWidget* filterWidget);

    /**
     * @brief getFilterPipeline
     * @return
     */
    FilterPipeline::Pointer getFilterPipeline();

    /**
    * @brief getStatusBar
    * @return
    */
    QStatusBar* getStatusBar();

    /**
     * @brief Returns a FilterPipeline Object with a new filter instance that has the input parameters copied
     * from the filter instance that is embedded in the PipelineFilterWidget instance. This function does NOT perform
     * a DEEP copy of the filter.
     * @return
     */
    FilterPipeline::Pointer getCopyOfFilterPipeline();

    /**
     * @brief setScrollArea
     * @param sa
     */
    void setScrollArea(QScrollArea* sa);
    /**
     * @brief setAutoScroll
     * @param scroll
     */
    void setAutoScroll(bool scroll) {m_AutoScroll = scroll;}
    /**
     * @brief hasAutoScroll
     * @return
     */
    bool hasAutoScroll() {return m_AutoScroll;}

    /**
     * @brief setAutoScrollMargin
     * @param margin
     */
    void setAutoScrollMargin(int margin) {m_AutoScrollMargin = margin;}

    /**
     * @brief getAutoScrollMargin
     * @return
     */
    int getAutoScrollMargin() {return m_AutoScrollMargin;}

    /**
     * @brief stopAutoScroll Stops the time so the Auto Scrolling will cease.
     */
    void stopAutoScroll();

    /**
     * @brief startAutoScroll Starts the QTimer in charge of auto scrolling the view
     */
    void startAutoScroll();

    /**
     * @brief shouldAutoScroll Figures out if the mouse position is in the margin area that would trigger an
     * autoscroll
     * @param pos
     * @return
     */
    bool shouldAutoScroll(const QPoint& pos);

    /**
     * @brief newEmptyPipelineViewLayout
     */
    void newEmptyPipelineViewLayout();

    /**
     * @brief resetLayout
     */
    void resetLayout();

    /**
    * @brief clearSelectedFilterWidgets
    */
    void clearSelectedFilterWidgets();

    /**
     * @brief setPipelineMessageObserver
     * @param pipelineMessageObserver
     */
    void setPipelineMessageObserver(QObject* pipelineMessageObserver);

    /**
     * @brief Sets the actions that will be displayed as a context menu
     * @param list
     */
    void setContextMenuActions(QList<QAction*> list);

    /**
     * @brief populatePipelineView
     * @param pipeline
     */
    void populatePipelineView(FilterPipeline::Pointer pipeline, int index);

    /**
     * @brief eventFilter
     * @return
     */
    bool eventFilter(QObject*, QEvent*);

    /**
    * @brief getSelectedFilterWidgets
    * @return
    */
    QList<PipelineFilterWidget*> getSelectedFilterWidgets();

    /**
    * @brief getDraggedFilterWidgets
    * @return
    */
    QList<PipelineFilterWidget*> getDraggedFilterWidgets();

    /**
     * @brief getActionRedo
     */
    QAction* getActionRedo();

    /**
     * @brief getActionUndo
     */
    QAction* getActionUndo();

  public slots:

    /**
    * @brief addUndoCommand
    */
    void addUndoCommand(QUndoCommand* cmd);

    /**
     * @brief addFilter
     * @param filter
     * @param index
     * @param allowUndo
     */
    void addFilter(AbstractFilter::Pointer filter, int index = -1, bool allowUndo = true);

    /**
     * @brief addFilter
     * @param filterClassName
     * @param index
     * param allowUndo
     */
    void addFilter(const QString& filterClassName, int index = -1, bool allowUndo = true);

    /**
     * @brief addFilter
     * @param filter
     * @param index
     * @param allowUndo
     */
    void addFilters(QList<AbstractFilter::Pointer> filters, int index = -1, bool allowUndo = true);

    /**
     * @brief addFilterWidget
     * @param fw
     * @param index
     * @param allowUndo
     */
    void addFilterWidget(PipelineFilterWidget* fw, int index = -1, bool allowUndo = true);

    /**
     * @brief addFilterWidget
     * @param fw
     * @param index
     * @param allowUndo
     */
    void addFilterWidgets(QList<PipelineFilterWidget*> filterWidgets, int index = -1, bool allowUndo = true);

    /**
     * @brief addFilterWidget
     * @param filterWidgets
     */
    void cutFilterWidgets(QList<PipelineFilterWidget*> filterWidgets, bool allowUndo = true);

    /**
     * @brief moveFilterWidget
     * @param fw
     * @param origin
     * @param destination
     */
    void moveFilterWidget(PipelineFilterWidget* fw, int origin, int destination/*, bool allowUndo = true*/);

    /**
     * @brief pasteFilters
     * @param filters
     */
    void pasteFilters(QList<AbstractFilter::Pointer> filters, bool allowUndo = true);

    /**
     * @brief pasteFilterWidgets
     * @param jsonString
     * @param index
     * @param allowUndo
     */
    void pasteFilterWidgets(const QString &jsonString, int index, bool allowUndo = true);

    /**
     * @brief removeFilterWidget
     * @param filterWidget
     * @param allowUndo
     */
    void removeFilterWidget(PipelineFilterWidget* filterWidget, bool allowUndo = true, bool deleteWidget = true);

    /**
     * @brief removeFilterWidget
     * @param filterWidgets
     * @param allowUndo
     */
    void removeFilterWidgets(QList<PipelineFilterWidget*> filterWidgets, bool allowUndo = true);

    /**
     * @brief setSelectedFilterWidget
     * @param w
     * @param modifiers
     */
    void setSelectedFilterWidget(PipelineFilterWidget* w, Qt::KeyboardModifiers modifiers);

    /**
     * @brief setStatusBar
     * @param statusBar
     */
    void setStatusBar(QStatusBar* statusBar);

    /**
     * @brief preflightPipeline
     */
    void preflightPipeline();

    /**
     * @brief doAutoScroll This does the actual scrolling of the Widget
     */
    void doAutoScroll();

    /**
    * @brief Write pipeline to a file
    */
    int writePipeline(QString filePath);

    /**
    * @brief Open pipeline to a file
    */
    int openPipeline(const QString& filePath, int index, const bool& setOpenedFilePath, const bool& changeTitle);

    /**
     * @brief reindexWidgetTitles
     */
    void reindexWidgetTitles();

    /**
     * @brief addSIMPLViewReaderFilter
     * @param filePath
     */
    void addSIMPLViewReaderFilter(const QString& filePath, int index);

    /**
     * @brief readPipelineFromFile
     * @param filePath
     * @return FilterPipeline::Pointer
     */
    FilterPipeline::Pointer readPipelineFromFile(const QString& filePath);

    /**
    * @brief clearWidgets
    */
    void clearWidgets(bool allowUndo = false);

    /**
    * @brief toRunningState
    */
    void toRunningState();

    /**
    * @brief toIdleState
    */
    void toIdleState();

    void showFilterHelp(const QString& className);

  signals:
    void addPlaceHolderFilter(QPoint p);
    void removePlaceHolderFilter();
    void preflightHasMessage(PipelineMessage msg);

    void pipelineOpened(QString& file, const bool& setOpenedFilePath, const bool& changeTitle);
    void pipelineHasErrorsSignal();
    void pipelineHasNoErrors();
    void pipelineIssuesCleared();
    void pipelineTitleUpdated(QString name);
    void pipelineChanged();

    void filterInputWidgetChanged(FilterInputWidget* widget);
    void filterInputWidgetNeedsCleared();

    void filterInputWidgetEdited();
    void preflightPipelineComplete();
    void preflightFinished(int err);

    void deleteKeyPressed(PipelineViewWidget* viewWidget);

    void moveCommandNeeded(PipelineFilterWidget* filterWidget, int originIndex, int destIndex, PipelineViewWidget* viewWidget);

    void filterWidgetsDropped(int insertIndex, Qt::KeyboardModifiers modifiers);

    void contextMenuRequested(PipelineViewWidget* widget, const QPoint &pos);

  protected:
    void setupGui();
    void dragEnterEvent(QDragEnterEvent* event);
    void dragLeaveEvent(QDragLeaveEvent* event);
    void dragMoveEvent(QDragMoveEvent* event);
    void dropEvent(QDropEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent *event);
    void focusInEvent(QFocusEvent* event);
    void focusOutEvent(QFocusEvent* event);

  protected slots:
    void handleFilterParameterChanged();
    void startDrag(QMouseEvent* event, PipelineFilterWidget *fw);

    void on_focusInEventStarted(QFocusEvent* event);
    void on_focusOutEventStarted(QFocusEvent* event);

    void requestContextMenu(const QPoint& pos);

  private:
    PipelineFilterWidget*               m_ShiftStart;
    QList<PipelineFilterWidget*>        m_DraggedFilterWidgets;
    QVBoxLayout*                        m_FilterWidgetLayout;
    int                                 m_FilterOrigPos;
    bool                                m_PipelineIsRunning;
    DropBoxWidget*                      m_DropBox;
    int                                 m_DropIndex;
    QLabel*                             m_EmptyPipelineLabel;
    QPoint                              m_LastDragPoint;
    QScrollArea*                        m_ScrollArea;
    QTimer                              m_autoScrollTimer;
    bool                                m_AutoScroll;
    int                                 m_AutoScrollMargin;
    int                                 m_autoScrollCount;
    QWidget*                            m_InputParametersWidget;
    QObject*                            m_PipelineMessageObserver;
    QMenu                               m_Menu;
    QStatusBar*                         m_StatusBar;
    QList<QAction*>                     m_MenuActions;
    QUndoStack*                         m_UndoStack;
    QAction*                            m_ActionUndo;
    QAction*                            m_ActionRedo;

    PipelineViewWidget(const PipelineViewWidget&); // Copy Constructor Not Implemented
    void operator=(const PipelineViewWidget&); // Operator '=' Not Implemented
};

#endif /* _PipelineViewWidget_H */
