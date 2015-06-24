#pragma once

enum ENUM_REPORT_TYPE
{ 
	// passenger
	ENUM_PAXLOG_REP=0,
	ENUM_DISTANCE_REP, 
	ENUM_QUEUETIME_REP,
	ENUM_DURATION_REP,
	ENUM_SERVICETIME_REP,
	ENUM_ACTIVEBKDOWN_REP,
	ENUM_PASSENGERNO_REP,

	// processor
	ENUM_UTILIZATION_REP,
	ENUM_THROUGHPUT_REP,
	ENUM_QUEUELENGTH_REP,
	ENUM_AVGQUEUELENGTH_REP,

	// space
	ENUM_PAXCOUNT_REP,
	ENUM_PAXDENS_REP,
	ENUM_SPACETHROUGHPUT_REP,
	ENUM_COLLISIONS_REP,

	// ac 
	ENUM_ACOPERATION_REP,

	// bag
	ENUM_BAGCOUNT_REP,
	ENUM_BAGWAITTIME_REP,
	ENUM_BAGDISTRIBUTION_REP,
	ENUM_BAGDELIVTIME_REP,

	// economic
	ENUM_ECONOMIC_REP,
	// fire evacution
	ENUM_FIREEVACUTION_REP,

	//conveyor
	ENUM_CONVEYOR_WAIT_LENGTH_REP,
	ENUM_CONVEYOR_WAIT_TIME_REP,
	ENUM_MISSFLIGHT_REP,

	//billboard
	ENUM_BILLBOARD_EXPOSUREINCIDENCE_REP,
	ENUM_BILLBOARD_LINKEDPROCINCREMENTVISIT_REP,

	//retail activity report
	ENUM_RETAIL_REP,
};
