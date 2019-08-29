/*
Plan Recognition as Planning Demo: Home domain
Copyright (C) 2012
Miquel Ramirez <miquel.ramirez@rmit.edu.au>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <widgets/AppWindow.hxx>
#include <iostream>
#include <QFileDialog>
#include <QStringList>
#include <widgets/HomeRenderer.hxx>
#include <widgets/RoomView.hxx>
#include <widgets/GoalSetView.hxx>
#include <QTextStream>
#include <model/SimHome.hxx>
#include <planning/Observer.hxx>
#include <string>
#include <action.hxx>
#include <model/Home.hxx>
#include <model/Room.hxx>
#include <model/Door.hxx>
#include <model/WalkTo.hxx>
#include <model/PickUp.hxx>
#include <model/Drop.hxx>
#include <model/OpenDoor.hxx>
#include <model/CloseDoor.hxx>
#include <model/StageProp.hxx>
#include <model/StagePropAction.hxx>


namespace UI
{

AppWindow::AppWindow( QWidget* parent, Qt::WindowFlags flags )
	: QMainWindow( parent, flags )
{

	setupWidgets();
	mHomeScene = new HomeRenderer;
	mGraphicDisplay->setScene( mHomeScene );

	mFileWatcher = new QFileSystemWatcher(this);
	connect(mFileWatcher, SIGNAL(fileChanged(const QString &)), this, SLOT(fileChanged(const QString &)));
	mFileWatcher->addPath("thor/observations.log");
}

AppWindow::~AppWindow()
{
}

void AppWindow::fileChanged(const QString & path)
{

	QFile f(path);
	if (!f.open(QFile::ReadOnly | QFile::Text)) return;
	QTextStream in(&f);	
	//mObsActView->logAction(in.readAll());

	std::string ObsName = in.readAll().toStdString();
	// for(auto act : mSimHome->planningDomain().actions() ){
	// 	std::cout << "compare : " << ObsName << " and "<<act->signature() << std::endl;
	// 	if (act->signature().compare( ObsName ) == 0){
	// 		mSimHome->observer()->actionExecuted(act->index());
	// 		break;
	// 	}
	// }
	
	ObsName.erase(std::remove(ObsName.begin(), ObsName.end(), '\n'), ObsName.end());
	for ( unsigned k = 0; k < mSimHome->home()->rooms().size(); k++ ){
		for ( int i = 0; i < mSimHome->home()->rooms()[k]->walkToActions().size(); i++ )
			{
				std::string stripsName = mSimHome->home()->rooms()[k]->walkToActions()[i]->strips()->signature();
                                std::cout << "compare : " << ObsName << " and "<< stripsName << std::endl;
				if( stripsName.compare( ObsName ) == 0 ) {
					mSimHome->home()->rooms()[k]->walkToActions()[i]->execute();
					break;
				}
			}
		for ( int i = 0; i < mSimHome->home()->rooms()[k]->pickUpActions().size(); i++ )
			{
				std::string stripsName = mSimHome->home()->rooms()[k]->pickUpActions()[i]->strips()->signature();
				if( stripsName.compare( ObsName ) == 0 ) {
					mSimHome->home()->rooms()[k]->pickUpActions()[i]->execute();
					break;
				}
					
			}
		for ( int i = 0; i < mSimHome->home()->rooms()[k]->dropActions().size(); i++ )
			{
					
				std::string stripsName = mSimHome->home()->rooms()[k]->dropActions()[i]->strips()->signature();
				if( stripsName.compare( ObsName ) == 0 ) {
					mSimHome->home()->rooms()[k]->dropActions()[i]->execute();
					break;
				}
					
			}

	}

	for ( unsigned k = 0; k < mSimHome->home()->doors().size(); k++ ){
		for ( int i = 0; i < mSimHome->home()->doors()[k]->openActions().size(); i++ )
			{
				std::string stripsName = mSimHome->home()->doors()[k]->openActions()[i]->strips()->signature();
				if( stripsName.compare( ObsName ) == 0 ) {
					mSimHome->home()->doors()[k]->openActions()[i]->execute();
					break;
				}
			}
		for ( int i = 0; i < mSimHome->home()->doors()[k]->closeActions().size(); i++ )
			{
				std::string stripsName = mSimHome->home()->doors()[k]->closeActions()[i]->strips()->signature();
				if( stripsName.compare( ObsName ) == 0 ) {
					mSimHome->home()->doors()[k]->closeActions()[i]->execute();
					break;
				}
			}
	}
	
	for ( unsigned k = 0; k < mSimHome->home()->props().size(); k++ )	{
		Application::StageProp* prop = mSimHome->home()->props()[k];
		for (  QMap< QString, Application::StagePropAction* >::Iterator propActionIt = prop->actions().begin();
			propActionIt != prop->actions().end(); propActionIt++ )
		{
			if ( !propActionIt.value()->checkPrecondition() ) continue;
			std::string stripsName = propActionIt.value()->strips()->signature();
			if( stripsName.compare( ObsName ) == 0 ) {
				propActionIt.value()->execute();
				break;
			}
		
		}
	}
	
	
}	

void	AppWindow::makeRoom( RoomView* v )
{
	mHomeScene->addItem( v );
	mGraphicDisplay->invalidateScene();
}

void	AppWindow::setupWidgets()
{
	// Main window
	if ( objectName().isEmpty() )
		setObjectName( QString::fromUtf8("ApplicationWindow") );
	resize( 1024, 768 );
       	setWindowTitle(QApplication::translate("ApplicationWindow", "Sim Home", 0, QApplication::UnicodeUTF8));

	makeWidgets();
 
        mAppMenuBar = new QMenuBar(this);
        mAppMenuBar->setObjectName(QString::fromUtf8("AppMenuBar"));
        mAppMenuBar->setGeometry(QRect(0, 0, 803, 25));

	makeFileMenu();
	makePRMenu();
	makeAboutMenu();

        mAppMenuBar->addAction(mFileMenu->menuAction());
	mAppMenuBar->addAction(mPRMenu->menuAction());
        mAppMenuBar->addAction(mAboutMenu->menuAction());
     
	setMenuBar(mAppMenuBar);

        mStatusBar = new QStatusBar(this);
        mStatusBar->setObjectName(QString::fromUtf8("mStatusBar"));
        setStatusBar(mStatusBar);

        QMetaObject::connectSlotsByName(this);
	QObject::connect( mLoadHomeAction, SIGNAL(triggered()), this, SLOT(onLoadHome()) );
}

void	AppWindow::makeWorldWidgets()
{
	QVBoxLayout* worldLayout = new QVBoxLayout( mWorldWidget );
	worldLayout->setObjectName(QString::fromUtf8("ObserverWidgetLayout"));
	worldLayout->setContentsMargins( 1, 1, 1, 1 );
	

	mFloorPlanWidget = new QGroupBox("Floor Plan", mWorldWidget);	
	mFloorPlanWidget->setObjectName( QString::fromUtf8("FloorPlanWidget"));

	mFloorPlanWidgetLayout = new QVBoxLayout(mFloorPlanWidget);
        mFloorPlanWidgetLayout->setObjectName(QString::fromUtf8("FloorPlanWidgetLayout"));
        mFloorPlanWidgetLayout->setContentsMargins(10, 10, 10, 10);

        mGraphicDisplay = new QGraphicsView(mFloorPlanWidget);
        mGraphicDisplay->setObjectName(QString::fromUtf8("GraphicDisplay"));
        mGraphicDisplay->setBaseSize(QSize(800, 600));

	mFloorPlanWidgetLayout->addWidget(mGraphicDisplay);

	worldLayout->addWidget( mFloorPlanWidget );
}

void	AppWindow::makeObserverWidgets()
{
	QVBoxLayout*	obsLayout = new QVBoxLayout( mObserverWidget );
	obsLayout->setObjectName(QString::fromUtf8("ObserverWidgetLayout"));
	obsLayout->setContentsMargins( 1, 1, 1, 1 );

	mGoalsView = new GoalSetView( "Goals", mObserverWidget );
	mGoalsView->setObjectName( QString::fromUtf8("GoalsView" ) );	
	mGoalsView->setMinimumWidth( 300 );

	mObsActView = new ObservationsView( "Observed Actions", mObserverWidget );
	mObsActView->setObjectName( QString::fromUtf8( "ObservedActions" ) );
	mObsActView->setMaximumHeight( 300 );
	mObsActView->setMaximumWidth( 400 );

	obsLayout->addWidget( mGoalsView );
	obsLayout->addWidget( mObsActView );
}

void	AppWindow::makeWidgets()
{
	mCentralWidget = new QWidget(this);
        mCentralWidget->setObjectName(QString::fromUtf8("mCentralWidget"));

        mHorizontalLayout = new QHBoxLayout(mCentralWidget);
        mHorizontalLayout->setObjectName(QString::fromUtf8("HorizontalLayout"));
        mHorizontalLayout->setContentsMargins(5, 5, 5, 5);

	mWorldWidget = new QWidget( mCentralWidget );
	makeWorldWidgets();	

	mObserverWidget = new QWidget( mCentralWidget );
	mObserverWidget->setMinimumWidth( 400 );
	mObserverWidget->setMaximumWidth( 400 );
	mObserverWidget->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Expanding );
	makeObserverWidgets();

	mHorizontalLayout->addWidget( mWorldWidget );
	mHorizontalLayout->addWidget( mObserverWidget );

        setCentralWidget(mCentralWidget);
}

void	AppWindow::makeFileMenu()
{
	mFileMenu = new QMenu(mAppMenuBar);
        mFileMenu->setObjectName(QString::fromUtf8("FileMenu"));
	mFileMenu->setTitle(QApplication::translate("ApplicationWindow", "File", 0, QApplication::UnicodeUTF8));

	mLoadHomeAction = new QAction(this);
        mLoadHomeAction->setObjectName(QString::fromUtf8("LoadHomeAction"));
	mLoadHomeAction->setText(QApplication::translate("ApplicationWindow", "Load Home...", 0, QApplication::UnicodeUTF8));

        mQuitAction = new QAction(this);
        mQuitAction->setObjectName(QString::fromUtf8("mQuitAction"));
	mQuitAction->setText(QApplication::translate("ApplicationWindow", "&Quit", 0, QApplication::UnicodeUTF8));

	mFileMenu->addAction(mLoadHomeAction);
        mFileMenu->addSeparator();
        mFileMenu->addAction(mQuitAction);

}

void	AppWindow::makePRMenu()
{
	mPRMenu = new QMenu( mAppMenuBar );
	mPRMenu->setObjectName( QString::fromUtf8("PRMenu"));
	mPRMenu->setTitle( QApplication::translate( "ApplicationWindow", "PR", 0, QApplication::UnicodeUTF8));

	mObsLevelMenu = new QMenu( mPRMenu );
	mObsLevelMenu->setObjectName( QString::fromUtf8("ObsLevelMenu") );
	mObsLevelMenu->setTitle( QApplication::translate( "ApplicationWindow", "Observation Level", 0, QApplication::UnicodeUTF8 ));

	QAction* obsLvlAction = new QAction( this );
	obsLvlAction->setObjectName( QString::fromUtf8("ObsLevel10") );
	obsLvlAction->setText( QApplication::translate( "ApplicationWindow", "10%", 0, QApplication::UnicodeUTF8 ));
	QObject::connect( obsLvlAction, SIGNAL( triggered()), this, SLOT( setObsLevel10() ) ); 
	mObsLevelMenu->addAction( obsLvlAction );

	obsLvlAction = new QAction( this );
	obsLvlAction->setObjectName( QString::fromUtf8("ObsLevel30") );
	obsLvlAction->setText( QApplication::translate( "ApplicationWindow", "30%", 0, QApplication::UnicodeUTF8 ));
	QObject::connect( obsLvlAction, SIGNAL( triggered()), this, SLOT( setObsLevel30() ) ); 
	mObsLevelMenu->addAction( obsLvlAction );

	obsLvlAction = new QAction( this );
	obsLvlAction->setObjectName( QString::fromUtf8("ObsLevel50") );
	obsLvlAction->setText( QApplication::translate( "ApplicationWindow", "50%", 0, QApplication::UnicodeUTF8 ));
	QObject::connect( obsLvlAction, SIGNAL( triggered()), this, SLOT( setObsLevel50() ) ); 
	mObsLevelMenu->addAction( obsLvlAction );

	obsLvlAction = new QAction( this );
	obsLvlAction->setObjectName( QString::fromUtf8("ObsLevel70") );
	obsLvlAction->setText( QApplication::translate( "ApplicationWindow", "70%", 0, QApplication::UnicodeUTF8 ));
	QObject::connect( obsLvlAction, SIGNAL( triggered()), this, SLOT( setObsLevel70() ) ); 
	mObsLevelMenu->addAction( obsLvlAction );

	obsLvlAction = new QAction( this );
	obsLvlAction->setObjectName( QString::fromUtf8("ObsLevel100") );
	obsLvlAction->setText( QApplication::translate( "ApplicationWindow", "100%", 0, QApplication::UnicodeUTF8 ));
	QObject::connect( obsLvlAction, SIGNAL( triggered()), this, SLOT( setObsLevel100() ) ); 
	mObsLevelMenu->addAction( obsLvlAction );

	mStartObsAction = new QAction( this );
	mStartObsAction->setObjectName( QString::fromUtf8("StartObserving") );
	mStartObsAction->setText( QApplication::translate("ApplicationWindow", "Start Observing...", 0, QApplication::UnicodeUTF8 ));
	QObject::connect( mStartObsAction, SIGNAL( triggered() ), mGoalsView, SLOT( freeze() ) );
	QObject::connect( mStartObsAction, SIGNAL( triggered() ), mObsActView, SLOT( clearActionLog() ) ); 

	mStopObsAction = new QAction( this );
	mStopObsAction->setObjectName( QString::fromUtf8("StopObserving") );
	mStopObsAction->setText( QApplication::translate("ApplicationWindow", "Stop Observing...", 0, QApplication::UnicodeUTF8 ));
	QObject::connect( mStopObsAction, SIGNAL( triggered() ), mGoalsView, SLOT( unFreeze() ) );

	mPRMenu->addMenu( mObsLevelMenu );
	mPRMenu->addAction( mStartObsAction );
	mPRMenu->addAction( mStopObsAction );
	
}

void	AppWindow::setObsLevel10()
{
	emit setObsLevel( 0.1f );
}

void	AppWindow::setObsLevel30()
{
	emit setObsLevel( 0.3f );
}

void	AppWindow::setObsLevel50()
{
	emit setObsLevel( 0.5f );
}

void	AppWindow::setObsLevel70()
{
	emit setObsLevel( 0.7f );
}

void	AppWindow::setObsLevel100()
{
	emit setObsLevel( 1.0f );
}

void	AppWindow::makeAboutMenu()
{
        mAboutMenu = new QMenu(mAppMenuBar);
        mAboutMenu->setObjectName(QString::fromUtf8("AboutMenu"));
	mAboutMenu->setTitle(QApplication::translate("ApplicationWindow", "About", 0, QApplication::UnicodeUTF8));
}

void	AppWindow::onLoadHome()
{
	QFileDialog dialog(this);

	dialog.setFileMode( QFileDialog::ExistingFile );
	dialog.setNameFilter(tr("XML documents (*.xml)"));	
	dialog.setViewMode(QFileDialog::Detail);
	dialog.setAcceptMode( QFileDialog::AcceptOpen );

	QStringList filenames;

	if ( dialog.exec() )
	{
		filenames = dialog.selectedFiles();
		emit loadHome(filenames.first() );	
	}
		
}

void	AppWindow::onHomeLoaded( const QPixmap& pic )
{
	mHomeScene->setFloorPlan( pic );
	mGraphicDisplay->invalidateScene();
	mGoalsView->unFreeze();	
}

}
