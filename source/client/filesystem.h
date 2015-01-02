#ifndef _TTRTS_FILESYSTEM_H_
#define _TTRTS_FILESYSTEM_H_

#include <string>
#include <chrono>

#include "game.h"

#define STRINGIFY(x) _STRINGIFY(x)
#define _STRINGIFY(x) #x

bool FileExists( const std::string& name );

void WaitForFile( const std::string& name, const std::chrono::milliseconds& time );

int runFromFilesystem( const std::string& gamefile );

bool OutputGameStateFile(CTTRTSGame &game, const std::string &gameDir);

std::string getMapsDir();
std::string getGamesDir();

#endif