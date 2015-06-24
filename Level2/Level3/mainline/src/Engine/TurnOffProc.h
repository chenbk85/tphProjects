#ifndef __TURNOFFPROC_H
#define __TURNOFFPROC_H
#pragma once
#include "landfieldprocessor.h"

class TurnOffProc :
	public LandfieldProcessor
{
public:
	TurnOffProc(void);
	virtual ~TurnOffProc(void);
	// do offset for all the position related data. be careful with the derived class.
	virtual void DoOffset( DistanceUnit _xOffset, DistanceUnit _yOffset ){}

	virtual void Rotate( DistanceUnit _degree ){}

	virtual void Mirror( const Path* _p ){}

	//processor type
	virtual int getProcessorType (void) const { return TurnoffProcessor; }
	//processor name
	virtual const char *getProcessorName (void) const { return m_strTypeName; }
	//processor i/o	
	//virtual int readSpecialField(ArctermFile& procFile);
	//virtual int writeSpecialField(ArctermFile& procFile) const;

	virtual bool HasProperty(ProcessorProperty propType)const;

protected:

public:
	const static CString m_strTypeName; // "RoundAbout"

};

#endif