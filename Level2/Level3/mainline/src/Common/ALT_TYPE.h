#pragma once

enum ALTOBJECT_TYPE
{ 
	ALT_UNKNOWN = 0,
	ALT_ATYPE_START,
	//airside
	ALT_RUNWAY  = ALT_ATYPE_START ,
	ALT_TAXIWAY, 
	ALT_STAND, 
	ALT_DEICEBAY,
	ALT_GROUNDROUTE,
	ALT_WAYPOINT,
	ALT_HOLD,
	ALT_AIRROUTE,
	ALT_SECTOR,
	ALT_SURFACE,
	ALT_STRUCTURE,
	ALT_WALLSHAPE,
	ALT_CONTOUR,

	ALT_STRETCH,
	ALT_INTERSECTIONS,
	ALT_VEHICLEPOOLPARKING,
	ALT_TRAFFICLIGHT,
	ALT_TOLLGATE,
	ALT_STOPSIGN,
	ALT_YIELDSIGN,//
	ALT_ROUNDABOUT,
	ALT_TURNOFF,
	ALT_OVERPASS,
	ALT_UNDERPASS,
	ALT_LANEADAPTER,
	ALT_CLOVERLEAF,
	ALT_LINEPARKING,
	ALT_NOSEINPARKING,
	ALT_PARKINGLOT,
	ALT_HELIPORT,
	ALT_CIRCLESTRETCH,
	ALT_DRIVEROAD,
	ALT_PARKINGPLACE,
	ALT_REPORTINGAREA,
	ALT_STARTPOSITION,
	ALT_MEETINGPOINT,
	ALT_APAXBUSSPOT,
	ALT_ABAGCARTSPOT,
	/*add airside types ahead */ ALT_ATYPE_END, //airside object type end, cannot over 10000

	//landside
	ALT_LTYPE_START = ALT_UNKNOWN + 10000,
	ALT_LSTRETCH = ALT_LTYPE_START,
	ALT_LINTERSECTION,
	ALT_LPARKINGLOT,
	ALT_LCURBSIDE,
	ALT_LBUSSTATION,
	ALT_LEXT_NODE, //external node
	ALT_LCROSSWALK,
	ALT_LWALKWAY,
	ALT_LROUNDABOUT,
	ALT_LPORTAL,
	ALT_LSTRETCHSEGMENT,
	ALT_LTRAFFICLIGHT,	
	ALT_LSTOPSIGN,
	ALT_LTOLLGATE,
	ALT_LYIELDSIGN,
	ALT_LTAXIQUEUE,
	ALT_LTAXIPOOL,
	ALT_LSPEEDSIGN,
	ALT_LDECISIONPOINT,
	ALT_LAREA,
	/*add landside types ahead */ALT_LTYPE_END,
};


