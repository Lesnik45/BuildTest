#define SAILS_COLOR_QUANTITY				9
#define SHIP_TYPES_QUANTITY					22
#define SHIP_TYPES_QUANTITY_WITH_FORT		23	// must be (SHIP_TYPES_QUANTITY + 1)

#define SHIP_TARTANE			0

#define SHIP_WAR_TARTANE		1

//6
#define SHIP_LUGGER				2
#define SHIP_SLOOP				3
#define SHIP_YACHT			    4

//5
#define SHIP_SCHOONER			5
#define SHIP_BARQUE				6
#define SHIP_CARAVEL			7

//4
#define SHIP_BARKENTINE			8
#define SHIP_BRIGANTINE			9
#define SHIP_FLEUT				10
#define SHIP_BRIG				11
#define SHIP_GALEON_L			12

//3
#define SHIP_CORVETTE			13
#define SHIP_GALEON_H			14
#define SHIP_PINNACE			15

//2
#define SHIP_FRIGATE			16

//1
#define SHIP_LINESHIP			17
#define SHIP_WARSHIP			18
#define SHIP_BATTLESHIP			19

#define SHIP_MANOWAR			20

//
#define SHIP_BOAT               21
#define SHIP_FORT               22



#define SHIP_NOTUSED			 1000
#define SHIP_NOTUSED_TYPE_NAME	"Not Used"
#define SHIP_NAME_NOTUSED		"Noname"

object  ShipsTypes[SHIP_TYPES_QUANTITY_WITH_FORT];  // RealShips
object	SailsColors[SAILS_COLOR_QUANTITY];


ref GetShipByType(int iType) { return &ShipsTypes[iType]; }  // пользовать GetRealShip