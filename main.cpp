/**
*
* Solution to course project # 7
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2025/2026
*
* @author Konstantin Lozanov
* @idnumber OMI0600655
* @compiler GCC
*
* Console implementation of the board game Rummikub.
*
*/

#include <iostream>
#include <cstdlib>
#include <ctime>

const int MAX_PLAYERS = 4;
const int MIN_PLAYERS = 2;

const int MAX_TILES = 106;
const int HAND_SIZE = 14;

const int MAX_HAND = 60; // allow growth if player steals jokers etc.
const int MAX_TABLE_COMBINATIONS = 30;
const int MAX_TILES_IN_COMBINATION = 20;

const int MAX_LINE = 2048;

enum Color {
	COLOR_BLACK = 0,
	COLOR_ORANGE = 1,
	COLOR_RED = 2,
	COLOR_BLUE = 3,
	COLOR_INVALID = -1
};

struct Tile {
	int number;
	Color color;
	bool isJoker;
};

struct Combination {
	Tile tiles[MAX_TILES_IN_COMBINATION];
	int count;
};

struct Player {
	Tile hand[MAX_HAND];
	int handCount;
	bool hasOpened;
};

bool areTwoStringsEqual(const char *string1, const char *string2) {
	while (*string1 != '\0' && *string2 != '\0') {
		if (*string1 != *string2) return false;
		string1++;
		string2++;
	}
	return *string1 == '\0' && *string2 == '\0';
}

void strConcat(char *dest, const char *src) {
	while (*dest != '\0') {
		dest++;
	}
	while (*src != '\0') {
		*dest = *src;
		dest++;
		src++;
	}
	*dest = '\0';
}

bool isAmountOfPlayersInvalid(const int players) {
	if (players < MIN_PLAYERS || players > MAX_PLAYERS) {
		std::cout << "The entered amount of players is invalid" << std::endl;
		return true;
	}
	return false;
}

void tileToString(const Tile &tile, char out[32]) {
	out[0] = '\0';
	if (tile.isJoker) {
		strConcat(out, "J(");
		if (tile.color == COLOR_RED) {
			strConcat(out, "red");
		}
		else {
			strConcat(out, "black");
		}
		strConcat(out, ")");
		return;
	}
	char num[8];
	num[0] = '\0';
	if (tile.number >= 10) {
		num[0] = char('0' + (tile.number / 10));
		num[1] = char('0' + (tile.number % 10));
		num[2] = '\0';
	}
	else {
		num[0] = char('0' + tile.number);
		num[1] = '\0';
	}
	strConcat(out, num);
	strConcat(out, "(");
	if (tile.color == COLOR_BLACK) {
		strConcat(out, "black");
	}
	else if (tile.color == COLOR_ORANGE) {
		strConcat(out, "orange");
	}
	else if (tile.color == COLOR_RED) {
		strConcat(out, "red");
	}
	else {
		strConcat(out, "blue");
	}
	strConcat(out, ")");
}

bool tilesEqual(const Tile &a, const Tile &b) {
	return a.isJoker == b.isJoker && a.number == b.number && a.color == b.color;
}

void swapTiles(Tile &a, Tile &b) {
	Tile temp = a;
	a = b;
	b = temp;
}

void shuffleTiles(Tile tiles[MAX_TILES], int count) {
	for (int i = count - 1; i > 0; i--) {
		int j = std::rand() % (i + 1);
		swapTiles(tiles[i], tiles[j]);
	}
}

void createFullPile(Tile pile[MAX_TILES], int &pileCount) {
	pileCount = 0;
	for (int copy = 0; copy < 2; copy++) {
		for (int color = 0; color < 4; color++) {
			for (int number = 1; number <= 13; number++) {
				pile[pileCount].number = number;
				pile[pileCount].color = (Color)color;
				pile[pileCount].isJoker = false;
				pileCount++;
			}
		}
	}
	pile[pileCount].number = 0;
	pile[pileCount].color = COLOR_RED;
	pile[pileCount].isJoker = true;
	pileCount++;
	pile[pileCount].number = 0;
	pile[pileCount].color = COLOR_BLACK;
	pile[pileCount].isJoker = true;
	pileCount++;
}

bool addTileToHand(Player &player, const Tile &tile) {
	if (player.handCount >= MAX_HAND) {
		return false;
	}
	player.hand[player.handCount] = tile;
	player.handCount++;
	return true;
}

bool removeTileFromHand(Player &player, const Tile &tile) {
	for (int i = 0; i < player.handCount; i++) {
		if (tilesEqual(player.hand[i], tile)) {
			for (int j = i; j < player.handCount - 1; j++) {
				player.hand[j] = player.hand[j + 1];
			}
			player.handCount--;
			return true;
		}
	}
	return false;
}

bool dealInitialHands(Player players[MAX_PLAYERS], int playersCount, Tile pile[MAX_TILES], int &pileCount) {
	for (int i = 0; i < playersCount; i++) {
		players[i].handCount = 0;
		players[i].hasOpened = false;
	}

	for (int round = 0; round < HAND_SIZE; round++) {
		for (int p = 0; p < playersCount; p++) {
			if (pileCount <= 0) {
				return false;
			}
			pileCount--;
			addTileToHand(players[p], pile[pileCount]);
		}
	}
	return true;
}

void printHand(const Player &player) {
	std::cout << "Hand (" << player.handCount << "):" << std::endl;
	for (int i = 0; i < player.handCount; i++) {
		char buf[32];
		tileToString(player.hand[i], buf);
		std::cout << "  " << (i + 1) << ": " << buf << std::endl;
	}
}

void printTable(const Combination table[MAX_TABLE_COMBINATIONS], int tableCount) {
	if (tableCount == 0) {
		std::cout << "Table is empty." << std::endl;
		return;
	}
	std::cout << "Table:" << std::endl;
	for (int i = 0; i < tableCount; i++) {
		std::cout << "  " << (i + 1) << ": ";
		for (int j = 0; j < table[i].count; j++) {
			char buf[32];
			tileToString(table[i].tiles[j], buf);
			std::cout << buf;
			if (j + 1 < table[i].count) {
				std::cout << " ";
			}
		}
		std::cout << std::endl;
	}
}

Color parseColorName(const char *name, int &consumed) {
	consumed = 0;
	if (name[0] == 'b' && name[1] == 'l' && name[2] == 'a' && name[3] == 'c' && name[4] == 'k') {
		consumed = 5;
		return COLOR_BLACK;
	}
	if (name[0] == 'o' && name[1] == 'r' && name[2] == 'a' && name[3] == 'n' && name[4] == 'g' && name[5] == 'e') {
		consumed = 6;
		return COLOR_ORANGE;
	}
	if (name[0] == 'r' && name[1] == 'e' && name[2] == 'd') {
		consumed = 3;
		return COLOR_RED;
	}
	if (name[0] == 'b' && name[1] == 'l' && name[2] == 'u' && name[3] == 'e') {
		consumed = 4;
		return COLOR_BLUE;
	}
	return COLOR_INVALID;
}

bool parseTileToken(const char *token, Tile &tile) {
	if (token[0] == '\0') {
		return false;
	}
	int i = 0;
	tile.isJoker = false;
	tile.number = 0;
	tile.color = COLOR_INVALID;

	if (token[i] == 'J') {
		tile.isJoker = true;
		tile.number = 0;
		i++;
	}
	else {
		if (token[i] < '0' || token[i] > '9') {
			return false;
		}
		int number = token[i] - '0';
		i++;
		if (token[i] >= '0' && token[i] <= '9') {
			number = number * 10 + (token[i] - '0');
			i++;
		}
		if (number < 1 || number > 13) {
			return false;
		}
		tile.number = number;
	}
	if (token[i] != '(') {
		return false;
	}
	i++;
	int consumed = 0;
	Color c = parseColorName(&token[i], consumed);
	if (c == COLOR_INVALID) {
		return false;
	}
	i += consumed;
	if (token[i] != ')') {
		return false;
	}
	i++;
	if (token[i] != '\0') {
		return false;
	}
	if (tile.isJoker && !(c == COLOR_RED || c == COLOR_BLACK)) {
		return false;
	}
	tile.color = c;
	return true;
}

bool isSpace(char ch) {
	return ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n';
}

void trimLeft(const char *&p) {
	while (*p != '\0' && isSpace(*p)) {
		p++;
	}
}

bool readToken(const char *&p, char out[64]) {
	trimLeft(p);
	if (*p == '\0') {
		out[0] = '\0';
		return false;
	}
	int idx = 0;
	while (*p != '\0' && !isSpace(*p) && *p != ';') {
		if (idx < 63) {
			out[idx] = *p;
			idx++;
		}
		p++;
	}
	out[idx] = '\0';
	return true;
}

bool parseTableLine(const char *line, Combination newTable[MAX_TABLE_COMBINATIONS], int &newTableCount) {
	newTableCount = 0;
	const char *p = line;
	while (true) {
		trimLeft(p);
		if (*p == '\0') {
			break;
		}
		if (newTableCount >= MAX_TABLE_COMBINATIONS) {
			return false;
		}
		newTable[newTableCount].count = 0;
		while (true) {
			trimLeft(p);
			if (*p == '\0' || *p == ';') {
				break;
			}
			char token[64];
			if (!readToken(p, token)) {
				break;
			}
			Tile t;
			if (!parseTileToken(token, t)) {
				return false;
			}
			if (newTable[newTableCount].count >= MAX_TILES_IN_COMBINATION) {
				return false;
			}
			newTable[newTableCount].tiles[newTable[newTableCount].count] = t;
			newTable[newTableCount].count++;
		}
		if (newTable[newTableCount].count == 0) {
			return false;
		}
		newTableCount++;
		trimLeft(p);
		if (*p == ';') {
			p++;
			continue;
		}
	}
	return true;
}

void bubbleSortInts(int arr[32], int n) {
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < n - i - 1; j++) {
			if (arr[j] > arr[j + 1]) {
				int temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
		}
	}
}

// -------------------- Validation + Joker assignment helpers --------------------

bool isSetValidWithJokerInfo(const Combination &comb, int &points,
	Tile representedForJokers[MAX_TILES_IN_COMBINATION], bool hasRep[MAX_TILES_IN_COMBINATION]) {
	points = 0;
	for (int i = 0; i < comb.count; i++) { hasRep[i] = false; }

	if (comb.count < 3 || comb.count > 4) return false;

	int nonJokerNumber = 0;
	bool hasNonJoker = false;
	bool usedColor[4] = {false, false, false, false};
	int jokerIdxs[4]; int jCount = 0;

	for (int i = 0; i < comb.count; i++) {
		if (comb.tiles[i].isJoker) {
			jokerIdxs[jCount++] = i;
			continue;
		}
		if (!hasNonJoker) {
			nonJokerNumber = comb.tiles[i].number;
			hasNonJoker = true;
		} else if (comb.tiles[i].number != nonJokerNumber) {
			return false;
		}
		int c = (int)comb.tiles[i].color;
		if (c < 0 || c > 3) return false;
		if (usedColor[c]) return false;
		usedColor[c] = true;
	}

	if (!hasNonJoker) return false;

	// Assign joker representations: same number, missing colors.
	// For a set, each joker can represent any missing color.
	for (int k = 0; k < jCount; k++) {
		for (int c = 0; c < 4; c++) {
			if (!usedColor[c]) {
				representedForJokers[jokerIdxs[k]].isJoker = false;
				representedForJokers[jokerIdxs[k]].number = nonJokerNumber;
				representedForJokers[jokerIdxs[k]].color = (Color)c;
				hasRep[jokerIdxs[k]] = true;
				usedColor[c] = true; // choose one missing color deterministically
				break;
			}
		}
		if (!hasRep[jokerIdxs[k]]) return false;
	}

	points = nonJokerNumber * comb.count;
	return true;
}

bool isRunValidWithJokerInfo(const Combination &comb, int &points,
	Tile representedForJokers[MAX_TILES_IN_COMBINATION], bool hasRep[MAX_TILES_IN_COMBINATION],
	int &bestStartOut, Color &runColorOut) {
	points = 0;
	for (int i = 0; i < comb.count; i++) { hasRep[i] = false; }
	bestStartOut = 0;
	runColorOut = COLOR_INVALID;

	if (comb.count < 3) return false;

	Color runColor = COLOR_INVALID;
	int nums[32]; int nCount = 0;
	int jokerIdxs[32]; int jCount = 0;

	for (int i = 0; i < comb.count; i++) {
		if (comb.tiles[i].isJoker) {
			jokerIdxs[jCount++] = i;
			continue;
		}
		if (runColor == COLOR_INVALID) runColor = comb.tiles[i].color;
		else if (comb.tiles[i].color != runColor) return false;

		nums[nCount++] = comb.tiles[i].number;
	}

	if (runColor == COLOR_INVALID) return false;

	bubbleSortInts(nums, nCount);
	for (int i = 0; i < nCount - 1; i++) {
		if (nums[i] == nums[i + 1]) return false;
	}

	int length = comb.count;
	bool possible = false;
	int bestStart = 0;
	for (int start = 1; start <= 13 - length + 1; start++) {
		int end = start + length - 1;
		bool ok = true;
		for (int i = 0; i < nCount; i++) {
			if (nums[i] < start || nums[i] > end) { ok = false; break; }
		}
		if (ok) { possible = true; bestStart = start; break; }
	}
	if (!possible) return false;

	// Determine which numbers are missing in the interval => joker representations
	bool present[14]; // 1..13
	for (int i = 0; i < 14; i++) present[i] = false;
	for (int i = 0; i < nCount; i++) present[nums[i]] = true;

	int nextMissing = bestStart;
	for (int k = 0; k < jCount; k++) {
		while (nextMissing <= bestStart + length - 1 && present[nextMissing]) nextMissing++;
		if (nextMissing > bestStart + length - 1) return false;
		representedForJokers[jokerIdxs[k]].isJoker = false;
		representedForJokers[jokerIdxs[k]].number = nextMissing;
		representedForJokers[jokerIdxs[k]].color = runColor;
		hasRep[jokerIdxs[k]] = true;
		present[nextMissing] = true;
		nextMissing++;
	}

	points = 0;
	for (int v = bestStart; v < bestStart + length; v++) points += v;

	bestStartOut = bestStart;
	runColorOut = runColor;
	return true;
}

bool isCombinationValid(const Combination &comb, int &points) {
	Tile rep[MAX_TILES_IN_COMBINATION];
	bool hasRep[MAX_TILES_IN_COMBINATION];
	int dummyStart = 0; Color dummyColor = COLOR_INVALID;

	int p1 = 0;
	int p2 = 0;
	bool setOk = isSetValidWithJokerInfo(comb, p1, rep, hasRep);
	bool runOk = isRunValidWithJokerInfo(comb, p2, rep, hasRep, dummyStart, dummyColor);

	if (setOk) { points = p1; return true; }
	if (runOk) { points = p2; return true; }
	return false;
}

// -------------------- Table multiset utilities --------------------

void flattenTable(const Combination table[MAX_TABLE_COMBINATIONS], int tableCount, Tile out[MAX_TILES], int &outCount) {
	outCount = 0;
	for (int i = 0; i < tableCount; i++) {
		for (int j = 0; j < table[i].count; j++) {
			out[outCount++] = table[i].tiles[j];
		}
	}
}

bool multisetContainsAll(const Tile pool[MAX_TILES], int poolCount, const Tile needed[MAX_TILES], int neededCount) {
	bool used[MAX_TILES];
	for (int i = 0; i < poolCount; i++) used[i] = false;

	for (int n = 0; n < neededCount; n++) {
		bool found = false;
		for (int p = 0; p < poolCount; p++) {
			if (!used[p] && tilesEqual(pool[p], needed[n])) {
				used[p] = true;
				found = true;
				break;
			}
		}
		if (!found) return false;
	}
	return true;
}

// diff = bigger - smaller (multiset). Returns false if smaller not subset of bigger.
bool getDifferenceAsTiles(const Tile bigger[MAX_TILES], int biggerCount,
	const Tile smaller[MAX_TILES], int smallerCount,
	Tile diff[MAX_TILES], int &diffCount) {

	bool used[MAX_TILES];
	for (int i = 0; i < biggerCount; i++) used[i] = false;

	for (int s = 0; s < smallerCount; s++) {
		bool found = false;
		for (int b = 0; b < biggerCount; b++) {
			if (!used[b] && tilesEqual(bigger[b], smaller[s])) {
				used[b] = true;
				found = true;
				break;
			}
		}
		if (!found) return false;
	}

	diffCount = 0;
	for (int b = 0; b < biggerCount; b++) {
		if (!used[b]) diff[diffCount++] = bigger[b];
	}
	return true;
}

// Collect possible replacements for a specific joker color based on OLD table combos that contain that joker.
// We return a list of tiles that joker could represent in at least one valid interpretation.
void collectPossibleJokerReplacementsOldTable(
	const Combination table[MAX_TABLE_COMBINATIONS], int tableCount,
	Color jokerColor,
	Tile out[MAX_TILES], int &outCount) {

	outCount = 0;
	for (int i = 0; i < tableCount; i++) {
		bool contains = false;
		for (int j = 0; j < table[i].count; j++) {
			if (table[i].tiles[j].isJoker && table[i].tiles[j].color == jokerColor) {
				contains = true;
				break;
			}
		}
		if (!contains) continue;

		// Try set interpretation
		{
			int pts = 0;
			Tile rep[MAX_TILES_IN_COMBINATION];
			bool hasRep[MAX_TILES_IN_COMBINATION];
			if (isSetValidWithJokerInfo(table[i], pts, rep, hasRep)) {
				for (int j = 0; j < table[i].count; j++) {
					if (table[i].tiles[j].isJoker && table[i].tiles[j].color == jokerColor && hasRep[j]) {
						out[outCount++] = rep[j];
					}
				}
			}
		}
		// Try run interpretation
		{
			int pts = 0;
			Tile rep[MAX_TILES_IN_COMBINATION];
			bool hasRep[MAX_TILES_IN_COMBINATION];
			int bestStart = 0;
			Color runColor = COLOR_INVALID;
			if (isRunValidWithJokerInfo(table[i], pts, rep, hasRep, bestStart, runColor)) {
				for (int j = 0; j < table[i].count; j++) {
					if (table[i].tiles[j].isJoker && table[i].tiles[j].color == jokerColor && hasRep[j]) {
						out[outCount++] = rep[j];
					}
				}
			}
		}
	}
}

// check if list contains tile (as multiset or set, we just do simple scan)
bool listContainsTile(const Tile arr[MAX_TILES], int n, const Tile &t) {
	for (int i = 0; i < n; i++) {
		if (tilesEqual(arr[i], t)) return true;
	}
	return false;
}

// -------------------- Move application (FULL RULES-ish) --------------------

bool applyMove(Player &player,
	const Combination table[MAX_TABLE_COMBINATIONS], int tableCount,
	const Combination newTable[MAX_TABLE_COMBINATIONS], int newTableCount,
	Combination outTable[MAX_TABLE_COMBINATIONS], int &outTableCount,
	bool isOpeningMove) {

	// Validate all combinations in new table
	for (int i = 0; i < newTableCount; i++) {
		int p = 0;
		if (!isCombinationValid(newTable[i], p)) {
			std::cout << "Invalid combination on table position " << (i + 1) << std::endl;
			return false;
		}
	}

	Tile oldTableTiles[MAX_TILES]; int oldCount = 0;
	flattenTable(table, tableCount, oldTableTiles, oldCount);

	Tile newTableTiles[MAX_TILES]; int newCount = 0;
	flattenTable(newTable, newTableCount, newTableTiles, newCount);

	// Pool = old table + player hand
	Tile unionPool[MAX_TILES]; int unionCount = 0;
	for (int i = 0; i < oldCount; i++) unionPool[unionCount++] = oldTableTiles[i];
	for (int i = 0; i < player.handCount; i++) unionPool[unionCount++] = player.hand[i];

	// New table must use only tiles from pool
	if (!multisetContainsAll(unionPool, unionCount, newTableTiles, newCount)) {
		std::cout << "New table contains tiles that you do not have." << std::endl;
		return false;
	}

	// Tiles added from hand = new - old
	Tile addedFromHand[MAX_TILES]; int addedCount = 0;
	if (!getDifferenceAsTiles(newTableTiles, newCount, oldTableTiles, oldCount, addedFromHand, addedCount)) {
		// This can fail only if old is not subset of new (i.e., some old tile removed).
		// Removing is allowed ONLY for jokers that are taken into hand after replacement.
		// We'll handle this via explicit removedFromTable below; so do not fail here.
		addedCount = 0; // will compute later safely
	}

	// Removed from table = old - new
	Tile removedFromTable[MAX_TILES]; int removedCount = 0;
	bool okRemoved = getDifferenceAsTiles(oldTableTiles, oldCount, newTableTiles, newCount, removedFromTable, removedCount);
	if (!okRemoved) {
		// This means new had tiles not in old; that's fine. The diff old-new should always work.
		// If it fails, something is inconsistent.
		std::cout << "Error while comparing tables." << std::endl;
		return false;
	}

	// Recompute addedFromHand robustly (new - old) without assuming subset
	{
		bool usedOld[MAX_TILES];
		for (int i = 0; i < oldCount; i++) usedOld[i] = false;
		addedCount = 0;

		for (int i = 0; i < newCount; i++) {
			bool matched = false;
			for (int j = 0; j < oldCount; j++) {
				if (!usedOld[j] && tilesEqual(newTableTiles[i], oldTableTiles[j])) {
					usedOld[j] = true;
					matched = true;
					break;
				}
			}
			if (!matched) {
				addedFromHand[addedCount++] = newTableTiles[i];
			}
		}
	}

	// Must play at least one tile from hand each turn (including joker replacement which uses a tile)
	if (addedCount == 0) {
		std::cout << "You must place at least one tile from your hand." << std::endl;
		return false;
	}

	// Opening move restriction: cannot change existing combinations, and must add >=30 points
	if (isOpeningMove) {
		// When table isn't empty already, opening move must not modify it
		// (If tableCount==0, this check passes naturally.)
		if (newTableCount < tableCount) {
			std::cout << "For your first move you cannot remove combinations." << std::endl;
			return false;
		}
		for (int i = 0; i < tableCount; i++) {
			if (newTable[i].count != table[i].count) {
				std::cout << "For your first move you cannot change existing combinations." << std::endl;
				return false;
			}
			for (int j = 0; j < table[i].count; j++) {
				if (!tilesEqual(newTable[i].tiles[j], table[i].tiles[j])) {
					std::cout << "For your first move you cannot change existing combinations." << std::endl;
					return false;
				}
			}
		}

		int openingPoints = 0;
		for (int i = tableCount; i < newTableCount; i++) {
			int p = 0;
			isCombinationValid(newTable[i], p);
			openingPoints += p;
		}
		if (openingPoints < 30) {
			std::cout << "Your first move must have total points at least 30." << std::endl;
			return false;
		}
	}

	// Handle removed tiles: only allowed removed tiles are jokers (joker replacement rule)
	int removedJokers = 0;
	Tile removedJokerTiles[2];
	for (int i = 0; i < removedCount; i++) {
		if (!removedFromTable[i].isJoker) {
			std::cout << "You cannot take non-joker tiles from the table into your hand." << std::endl;
			return false;
		}
		if (removedJokers < 2) removedJokerTiles[removedJokers] = removedFromTable[i];
		removedJokers++;
	}
	if (removedJokers > 2) {
		std::cout << "Invalid: too many jokers removed." << std::endl;
		return false;
	}

	// Require that each removed joker is actually replaceable by one of the added hand tiles,
	// based on what that joker could represent in the OLD table.
	for (int r = 0; r < removedJokers; r++) {
		Tile possible[MAX_TILES]; int possibleCount = 0;
		collectPossibleJokerReplacementsOldTable(table, tableCount, removedJokerTiles[r].color, possible, possibleCount);

		bool matched = false;
		for (int a = 0; a < addedCount; a++) {
			if (addedFromHand[a].isJoker) continue; // replacing a joker with a joker doesn't make sense
			if (listContainsTile(possible, possibleCount, addedFromHand[a])) {
				matched = true;
				break;
			}
		}
		if (!matched) {
			std::cout << "Joker replacement invalid: you must replace the joker with the correct tile." << std::endl;
			return false;
		}
	}

	// Now ensure player actually has all addedFromHand tiles in hand
	if (!multisetContainsAll(player.hand, player.handCount, addedFromHand, addedCount)) {
		std::cout << "You do not have all tiles you are trying to play from your hand." << std::endl;
		return false;
	}

	// Apply: remove played tiles from hand
	for (int i = 0; i < addedCount; i++) {
		if (!removeTileFromHand(player, addedFromHand[i])) {
			std::cout << "Internal error: could not remove tile from hand." << std::endl;
			return false;
		}
	}

	// Apply: add removed jokers to hand (joker replacement reward)
	for (int r = 0; r < removedJokers; r++) {
		if (!addTileToHand(player, removedJokerTiles[r])) {
			std::cout << "Hand is full; cannot take joker." << std::endl;
			return false;
		}
	}

	// Commit table
	outTableCount = newTableCount;
	for (int i = 0; i < newTableCount; i++) outTable[i] = newTable[i];

	return true;
}

// -------------------- Misc game functions --------------------

void drawOneTile(Player &player, Tile pile[MAX_TILES], int &pileCount) {
	if (pileCount <= 0) {
		std::cout << "No tiles left to draw." << std::endl;
		return;
	}
	pileCount--;
	addTileToHand(player, pile[pileCount]);
	std::cout << "You drew 1 tile." << std::endl;
}

int calculateHandPenalty(const Player &player) {
	int sum = 0;
	for (int i = 0; i < player.handCount; i++) {
		if (player.hand[i].isJoker) sum += 30;
		else sum += player.hand[i].number;
	}
	return sum;
}

void printHelp() {
	std::cout << "Commands:" << std::endl;
	std::cout << "  show  - show your hand and the table" << std::endl;
	std::cout << "  play  - enter a new table state (combinations separated by ';')" << std::endl;
	std::cout << "  draw  - draw one tile and end turn" << std::endl;
	std::cout << "  pass  - same as draw" << std::endl;
	std::cout << "  help  - show this help" << std::endl;
	std::cout << "  quit  - quit the game" << std::endl;
	std::cout << std::endl;
	std::cout << "Tile format examples: 10(black) 3(red) J(red)" << std::endl;
	std::cout << "Table input example: 1(black) 2(black) 3(black); 7(red) 7(blue) 7(orange)" << std::endl;
}

bool readLine(char out[MAX_LINE]) {
	std::cin.getline(out, MAX_LINE);
	if (std::cin.eof()) {
		out[0] = '\0';
		return false;
	}
	if (std::cin.fail()) {
		std::cin.clear();
		out[0] = '\0';
	}
	return true;
}

void toLowerCase(char *s) {
	while (*s != '\0') {
		if (*s >= 'A' && *s <= 'Z') *s = char(*s - 'A' + 'a');
		s++;
	}
}

bool isOnlySpaces(const char *s) {
	while (*s != '\0') {
		if (!isSpace(*s)) return false;
		s++;
	}
	return true;
}

bool takeTurn(Player &player, int playerIndex,
	Combination table[MAX_TABLE_COMBINATIONS], int &tableCount,
	Tile pile[MAX_TILES], int &pileCount) {

	std::cout << std::endl;
	std::cout << "Player " << (playerIndex + 1);
	if (!player.hasOpened) std::cout << " (first move needs 30+ points)";
	std::cout << std::endl;

	printHelp();

	while (true) {
		std::cout << "> ";
		char line[MAX_LINE];
		if (!readLine(line)) return false;
		if (isOnlySpaces(line)) continue;
		toLowerCase(line);

		if (areTwoStringsEqual(line, "show")) {
			printHand(player);
			printTable(table, tableCount);
			continue;
		}
		if (areTwoStringsEqual(line, "help")) {
			printHelp();
			continue;
		}
		if (areTwoStringsEqual(line, "draw") || areTwoStringsEqual(line, "pass")) {
			drawOneTile(player, pile, pileCount);
			return true;
		}
		if (areTwoStringsEqual(line, "quit")) {
			return false;
		}
		if (areTwoStringsEqual(line, "play")) {
			std::cout << "Enter the full table after your move:" << std::endl;
			std::cout << "(you may rearrange existing combinations after opening)" << std::endl;
			std::cout << "> ";
			char tableLine[MAX_LINE];
			if (!readLine(tableLine)) return false;
			if (isOnlySpaces(tableLine)) {
				std::cout << "Empty input." << std::endl;
				continue;
			}

			Combination newTable[MAX_TABLE_COMBINATIONS];
			int newTableCount = 0;
			if (!parseTableLine(tableLine, newTable, newTableCount)) {
				std::cout << "Could not parse the table input." << std::endl;
				continue;
			}

			Combination outTable[MAX_TABLE_COMBINATIONS];
			int outCount = 0;
			bool opening = !player.hasOpened;
			if (!applyMove(player, table, tableCount, newTable, newTableCount, outTable, outCount, opening)) {
				continue;
			}

			for (int i = 0; i < outCount; i++) table[i] = outTable[i];
			tableCount = outCount;
			player.hasOpened = true;
			std::cout << "Move accepted." << std::endl;
			return true;
		}

		std::cout << "Unknown command. Type 'help'." << std::endl;
	}
}

int main() {
	std::srand((unsigned)std::time(nullptr));

	int numPlayers;
	std::cout << "How many players will play (2-4) -> ";
	std::cin >> numPlayers;
	if (isAmountOfPlayersInvalid(numPlayers)) return 0;
	std::cin.ignore(MAX_LINE, '\n');

	Tile pile[MAX_TILES];
	int pileCount = 0;
	createFullPile(pile, pileCount);
	shuffleTiles(pile, pileCount);

	Player players[MAX_PLAYERS];
	if (!dealInitialHands(players, numPlayers, pile, pileCount)) {
		std::cout << "Not enough tiles to start the game." << std::endl;
		return 0;
	}

	Combination table[MAX_TABLE_COMBINATIONS];
	int tableCount = 0;

	int current = 0;
	bool running = true;
	while (running) {
		bool ok = takeTurn(players[current], current, table, tableCount, pile, pileCount);
		if (!ok) { running = false; break; }

		if (players[current].handCount == 0) {
			std::cout << std::endl;
			std::cout << "Player " << (current + 1) << " wins by using all tiles!" << std::endl;
			break;
		}

		if (pileCount == 0) {
			bool someoneWon = false;
			for (int p = 0; p < numPlayers; p++) {
				if (players[p].handCount == 0) { someoneWon = true; break; }
			}
			if (!someoneWon) {
				int bestPlayer = 0;
				int bestScore = calculateHandPenalty(players[0]);
				for (int p = 1; p < numPlayers; p++) {
					int score = calculateHandPenalty(players[p]);
					if (score < bestScore) { bestScore = score; bestPlayer = p; }
				}
				std::cout << std::endl;
				std::cout << "No tiles left in the pile." << std::endl;
				std::cout << "Winner is player " << (bestPlayer + 1)
					<< " with smallest hand value: " << bestScore
					<< " (joker counts as 30)." << std::endl;
				break;
			}
		}

		current++;
		if (current >= numPlayers) current = 0;
	}

	std::cout << "Game ended." << std::endl;
	return 0;
}
