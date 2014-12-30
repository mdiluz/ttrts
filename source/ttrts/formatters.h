#ifndef _TTRTS_FORMATTERS_H_
#define _TTRTS_FORMATTERS_H_

#include "order.h"
#include "game.h"
#include "unit.h"

#include "string.h"

#define GAME_POS_FORMATTER "[%u,%u]"

#define GAME_HEADER_FORMATTER "NAME:%s\nSIZE:" GAME_POS_FORMATTER "\nTURN:%u\nWALL:%s"
#define GAME_HEADER_DELIMITER "~~~~\n"

#define UNIT_FORMATTER "UNIT:%u pl:%u vs:%c dr:%c ps:" GAME_POS_FORMATTER

// order <--> string conversion functions
std::string 	GetStringFromOrder(const SOrder & order );
SOrder 			GetOrderFromString( const std::string& order );

// game <--> string conversion functions
CTTRTSGame       GetGameFromString( const std::string& input );
std::string      GetStringFromGame( const CTTRTSGame& game );

// unit <--> string conversion functions
std::string 	GetStringFromUnit(const CUnit& unit );
CUnit 		GetUnitFromString(const std::string& unit );

#endif