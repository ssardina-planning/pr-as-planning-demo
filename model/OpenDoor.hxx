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

#ifndef __OPEN_DOOR__
#define __OPEN_DOOR__

#include <QObject>
#include <strips_prob.hxx>
#include <fluent.hxx>
#include <action.hxx>

using aptk::STRIPS_Problem;
using aptk::Fluent;
using aptk::Fluent_Ptr_Vec;
using aptk::Action;

namespace Application
{

class Door;
class Room;

class OpenDoor : public QObject
{
	Q_OBJECT
public:
	OpenDoor( Door* d, Room* r );
	virtual ~OpenDoor();

	bool		checkPrecondition();
	
	const QString&	doorName() const;
	const QString&	roomName() const;
	
	void		makeSTRIPSAction( STRIPS_Problem& p );
	Action*         strips() { return mSTRIPSAction; }
signals:
	
	void	executed( unsigned index );

public	slots:

	void	execute();

protected:
	Door*			mDoor;
	Room*			mRoom;
	Action*			mSTRIPSAction;
	Fluent_Ptr_Vec		mPrecFluents;
	Fluent_Ptr_Vec		mAddFluents;
	Fluent_Ptr_Vec		mDelFluents;

};

}


#endif // OpenDoor.hxx
