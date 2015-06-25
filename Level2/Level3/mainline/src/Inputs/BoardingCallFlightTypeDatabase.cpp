#include "StdAfx.h"
#include "BoardingCallFlightTypeDatabase.h"

BoardingCallFlightTypeEntry::BoardingCallFlightTypeEntry(void)
{
}


BoardingCallFlightTypeEntry::~BoardingCallFlightTypeEntry(void)
{
}

BoardingCallFlightTypeDatabase::BoardingCallFlightTypeDatabase(void)
{
}


BoardingCallFlightTypeDatabase::~BoardingCallFlightTypeDatabase(void)
{
}

void BoardingCallFlightTypeDatabase::deleteItem( ConstraintEntry* pConst )
{
	removeItem(pConst);
	delete pConst;
}



void BoardingCallFlightTypeDatabase::AddFlight(FlightConstraint* pInFlightConst, InputTerminal* _pInTerm)
{
	FlightConstraint* pFlightConstraint = new FlightConstraint();
	if(pInFlightConst)
	{
		*pFlightConstraint = *pInFlightConst;
	}
	else
	{
		pFlightConstraint->initDefault(); /* Set 'Flight Type': DEFAULT */
	}
	BoardingCallFlightTypeEntry* flightEntry = new BoardingCallFlightTypeEntry();
	flightEntry->initialize(pFlightConstraint, NULL);
	flightEntry->GetStandDatabase()->AddStand(NULL, _pInTerm);
	addEntry(flightEntry, true);/* Replace if exists. */
}

void BoardingCallFlightTypeDatabase::AddFlightFromOld(FlightConWithProcIDDatabase* pOldConWithProcIDDB, InputTerminal* _pInTerm)
{
	int count = pOldConWithProcIDDB->getCount();
	for(int i=0; i<count; i++)
	{
		ConstraintWithProcIDEntry* pConstWithProcIDEntry = (ConstraintWithProcIDEntry*)pOldConWithProcIDDB->getItem(i);
		FlightConstraint* pFlightConstraint = new FlightConstraint();
		*pFlightConstraint = *((FlightConstraint*)pConstWithProcIDEntry->getConstraint());
		BoardingCallFlightTypeEntry* flightEntry = new BoardingCallFlightTypeEntry();
		flightEntry->initialize(pFlightConstraint, NULL);
		flightEntry->GetStandDatabase()->AddStandFromOld(pConstWithProcIDEntry, _pInTerm);
		addEntry(flightEntry, true); /* Replace if constraint pFlightConstraint exists. */
	}
}

void BoardingCallFlightTypeDatabase::readDatabase(ArctermFile& p_file, InputTerminal* _pInTerm, int fltTypeCount)
{
	clear();
	for(int i=0; i<fltTypeCount; i++)
	{
		p_file.getLine();
		FlightConstraint fltConst;
		fltConst.SetAirportDB( _pInTerm->m_pAirportDB );
		fltConst.readConstraint (p_file);
		AddFlight(&fltConst, _pInTerm);

		int standCount;
		p_file.getInteger(standCount);
		((BoardingCallFlightTypeEntry*)getItem(i))->GetStandDatabase()->readDatabase(p_file, _pInTerm, standCount);
	}
}

void BoardingCallFlightTypeDatabase::writeDatabase(ArctermFile& p_file, char* szLabel, InputTerminal* _pInTerm)
{
	int flightCount = getCount();
	for(int i=0; i<flightCount; i++)
	{
		BoardingCallFlightTypeEntry* pFlightEntry = (BoardingCallFlightTypeEntry*)getItem(i);
		FlightConstraint* pFlightConst = (FlightConstraint*)(pFlightEntry->getConstraint());
		pFlightConst->writeConstraint(p_file);

		int standCount = pFlightEntry->GetStandCount();
		p_file.writeInt(standCount);
		p_file.writeLine();
		
		pFlightEntry->GetStandDatabase()->writeDatabase(p_file, _pInTerm);
	}
}