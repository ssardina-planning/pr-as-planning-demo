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

#include <model/SimHome.hxx>
#include <model/Home.hxx>
#include <cstdlib>
#include <iostream>
#include <QApplication>
#include <widgets/RoomView.hxx>
#include <model/Room.hxx>
#include <model/Door.hxx>
#include <model/Agent.hxx>
#include <model/StageProp.hxx>
#include <planning/Observer.hxx>
#include <fstream>      // std::fstream


namespace Application
{

SimHome::SimHome()
	: mHome( NULL )
{
	mObserver = new Planning::Observer( mDomain );
	QObject::connect( this, SIGNAL(  updateInitialState( aptk::Fluent_Vec& ) ),
			mObserver, SLOT( updateInitialState( aptk::Fluent_Vec& ) ) );
	QObject::connect( mObserver, SIGNAL( requestInitialStateUpdate() ),
			this, SLOT( initialStateUpdateRequested() ) );
}

SimHome::~SimHome()
{
	delete mObserver;
}

void	SimHome::quit()
{
	std::cout << "Quitting Sim Home" << std::endl;
	std::exit(0);
}

void	SimHome::loadHome( const QString& homePath )
{
	std::cout << "Loading Home from xml document: " << homePath.toStdString() << std::endl;
	if ( mHome ) 
		delete	mHome;
	mHome = new Home;
	if ( Home::load( homePath, mHome ) )
		emit onHomeLoaded( mHome->floorPlan() );
	for ( unsigned k = 0; k < mHome->rooms().size(); k++ )
	{
		UI::RoomView* v =  new UI::RoomView( mHome->rooms()[k], NULL );
		QObject::connect( mHome->rooms()[k], SIGNAL( showAgent() ), v, SLOT( showAgent() ) );
		QObject::connect( mHome->rooms()[k], SIGNAL( hideAgent() ), v, SLOT( hideAgent() ) );
		emit makeRoom( v );
	}
	
	Agent::instance().makeSTRIPSFluents(mDomain);
	for ( unsigned k = 0; k < mHome->rooms().size(); k++ )
		mHome->rooms()[k]->makeSTRIPSFluents(mDomain);
	for ( unsigned k = 0; k < mHome->doors().size(); k++ )
		mHome->doors()[k]->makeSTRIPSFluents(mDomain);
	for ( unsigned k = 0; k < mHome->props().size(); k++ )
		mHome->props()[k]->makeSTRIPSFluents(mDomain);
	for ( unsigned k = 0; k < mHome->rooms().size(); k++ )
		mHome->rooms()[k]->makeSTRIPSActions( mDomain, mObserver );
	for ( unsigned k = 0; k < mHome->doors().size(); k++ )
		mHome->doors()[k]->makeSTRIPSActions( mDomain, mObserver );
	for ( unsigned k = 0; k < mHome->props().size(); k++ )
		mHome->props()[k]->makeSTRIPSActions( mDomain, mObserver );

	std::cout << "STRIPS fluents: " << mDomain.fluents().size() << std::endl;
	std::cout << "STRIPS actions: " << mDomain.actions().size() << std::endl;

	std::fstream fs;
	fs.open ("thor/available_actions.log", std::fstream::in | std::fstream::out | std::fstream::app);

	mDomain.print_actions(fs);

	fs.close();

}

void	SimHome::initialStateUpdateRequested()
{
	aptk::Fluent_Vec evalResult;
	Agent::instance().evalSTRIPSFluents( evalResult );	
	for ( unsigned k = 0; k < mHome->rooms().size(); k++ )
		mHome->rooms()[k]->evalSTRIPSFluents( evalResult );
	for ( unsigned k = 0; k < mHome->doors().size(); k++ )
		mHome->doors()[k]->evalSTRIPSFluents( evalResult );
	emit updateInitialState( evalResult );
}

}

