#pragma once
enum
{
	VM_SELECT_ALTOBJECT =		  (WM_USER + 2048),
	VM_DELECT_ALTOBJECT,				// + 2049
	VM_CHANGENAME_ALTOBJECT,			// + 2050
	VM_NEW_MULT_ALTOBJECTS,				// + 2051
	VM_NEW_SINGLE_ALTOBJECT ,			// + 2052
	VM_MODIFY_ALTOBJECT =         (WM_USER + 2503),
	VM_MODIFY_AIRSIDEGROUND,			// + 2504

	MULTIREPORT_SHOWREPORT,				// + 2505
	VM_UPDATA_ALTOBJECTS,				// + 2506
	AIRSIDEREPORT_SHOWREPORT,			// + 2507

	VM_MODIFY_HOLDSHORTLINE,			// + 2508

	VM_UPDATE_STRETCH,					// + 2509
	VM_MODIFY_AIRROUTE,					// + 2510
	VM_SET_AIRSIDEPLACEMARKER,			// + 2511
	VM_SET_AIRSIDEPLACEMARKERLINE,		// + 2512
	VM_REMOVE_AIRSIDEPLACEMARKER,		// + 2513
	VM_REMOVE_AIRSIDEPLACEMARKERLINE,	// + 2514
	VM_ALIGN_AIRSIDE_MARKS,				// + 2515

	VM_ONBOARD_LAYOUT_DEFAULT,			// + 2516

	VM_MODIFY_AIRPORT,					// + 2517
	VM_ADD_OR_DELETE_AIRPORT,			// + 2518

	VM_GENERATE_REPORT,					// + 2519

	VM_GLOBALUNIT_CHANGER,				// + 2520

	//Floor Change
	VM_NEW_ONE_FLOOE,
	VM_DELETE_ONE_FLOOR,
	VM_ACTIVE_ONE_FLOOR,				// + 2524
	VM_UPDATE_ONE_FLOOR,				// + 2522
	VM_UPDATE_ALL_FLOORS,				// + 2521
	VM_UPDATE_FLOOR_ALTITUDE,			// + 2523
	
	VM_SELECT_LAYOUTOBJECT,
	VM_REDRAW,

	VM_UPDATE_COMMAND, //update a edit operation
	VM_UPDATE_ALTOBJECT3D,  //update a object in 3d scene

	VM_RELOAD_PAX_DISPLAY_PROPERTY,     // + 2525
	VM_RELOAD_PAX_TAG_PROPERTY,         // + 2526

	VM_RELOAD_FLIGHT_DISPLAY_PROPERTY,  // + 2527
	VM_RELOAD_FLIGHT_TAG_PROPERTY,      // + 2528
	VM_RELOAD_VEHICLE_DISPLAY_PROPERTY, // + 2529
	VM_RELOAD_VEHICLE_TAG_PROPERTY,     // + 2530

	VM_RELOAD_LANDSIDE_VEHICLE_DISPLAY_PROPERTY, // + 2529
	VM_RELOAD_LANDSIDE_VEHICLE_TAG_PROPERTY,     // + 2530

	VM_UPDATETRACE,


	AIRSIDEREPORT_DISLISTVIEW,			// + 2531
	VM_PROCESSORTYPE_CHECK,   // 2532

	VM_UPDATE_TRAFFICLIGHT,  // 2533
	VM_UPDATE_LANDSIDE_LAYOUTOPTION,  // 2534
    VM_COMPARATIVEREPORT_DISLISTVIEW, // 2535
    VM_COMPARATIVEREPORT_SHOWREPORT,  // 2536

	VM_MODIFY_AIRROUTENAME 
};
