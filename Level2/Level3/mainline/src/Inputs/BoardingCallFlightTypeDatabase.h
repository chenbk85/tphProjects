#pragma once
#include "FLT_DB.h"
#include "con_db.h"
#include "BoardingCallStandDatabase.h"
#include "FlightConWithProcIDDatabase.h"

class BoardingCallFlightTypeEntry : public ConstraintEntry
{
protected:
	BoardingCallStandDatabase m_standDB;
public:
	BoardingCallFlightTypeEntry();
	~BoardingCallFlightTypeEntry();
public:
	int GetStandCount(){ return (int)m_standDB.getCount(); }
	BoardingCallStandDatabase* GetStandDatabase(){ return &m_standDB; }
};


class BoardingCallFlightTypeDatabase : public FlightConstraintDatabase
{
public:
	BoardingCallFlightTypeDatabase();
	~BoardingCallFlightTypeDatabase();

public:
	void AddFlight(FlightConstraint* pFlight, InputTerminal* _pInTerm);
	void AddFlightFromOld(FlightConWithProcIDDatabase* pOldStage, InputTerminal* _pInTerm);

	void readDatabase(ArctermFile& p_file, InputTerminal* _pInTerm, int fltTypeCount);
	void writeDatabase(ArctermFile& p_file, char* szLabel, InputTerminal* _pInTerm);
	virtual void deleteItem(ConstraintEntry* pConst);
};

