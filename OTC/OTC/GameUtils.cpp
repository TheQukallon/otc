#include "pch.h"
#include "GameUtils.h"

void GameUtils::ExecuteAtConsole (const char* cmd) {
	GameUtils::ExecuteInterface (this, cmd);
}

void GameUtils::PrintToConsole (const char* message, int preNewLineCount, int postNewLineCount) {
	PrintNewLineToConsole (preNewLineCount);
	GameUtils::PrintInterface (message);
	PrintNewLineToConsole (postNewLineCount);
}

void GameUtils::PrintNewLineToConsole (int newLineCount) {
	for (int lineIterator = 0; lineIterator < newLineCount; lineIterator++) GameUtils::PrintInterface ("\n");
}