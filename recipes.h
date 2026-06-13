#pragma once
#include "libraries.h"
#include "InventorySlot.h"
//#include "json.hpp"
#include <fstream>

string recipespath = "recipes.json";

//using json = nlohmann::json;

template <typename T>
bool equals(T* item1, T* item2, int size) {
	bool same = true;
	for (int i = 0; i < size; i++) {
		if(item1[i] != item2[i]) {
			same = false;
			break;
		}
	}
	return same;
}

struct ItemResult {
	Item item;
	int count;
};

struct RecipeElement {
	Item grid[3][3];
	ItemResult result;
};

class Recipe {

private:
	vector<RecipeElement> recipes = {
		{{{OAK_WOOD, AIR, AIR},
		  {AIR,	     AIR, AIR},
		  {AIR,	     AIR, AIR}},				{OAK_PLANK, 4}},

		{{{AIR,      OAK_WOOD, AIR},
		  {AIR,	     AIR, AIR},
		  {AIR,	     AIR, AIR}},				{OAK_PLANK, 4}},

		{{{AIR,		 AIR, OAK_WOOD},
		  {AIR,	     AIR, AIR},
		  {AIR,	     AIR, AIR}},				{OAK_PLANK, 4}},

		{{{AIR,	     AIR, AIR},
		  {OAK_WOOD, AIR, AIR},
		  {AIR,	     AIR, AIR}},				{OAK_PLANK, 4}},

		{{{AIR,	     AIR, AIR},
		  {AIR,      OAK_WOOD, AIR},
		  {AIR,	     AIR, AIR}},				{OAK_PLANK, 4}},

		{{{AIR,	     AIR, AIR},
		  {AIR,		 AIR, OAK_WOOD},
		  {AIR,	     AIR, AIR}},				{OAK_PLANK, 4}},

		{{{AIR,	     AIR, AIR},
		  {AIR,	     AIR, AIR},
		  {OAK_WOOD, AIR, AIR}},				{OAK_PLANK, 4}},

		{{{AIR,	     AIR, AIR},
		  {AIR,	     AIR, AIR},
		  {AIR,      OAK_WOOD, AIR}},			{OAK_PLANK, 4}},

		{{{AIR,	     AIR, AIR},
		  {AIR,	     AIR, AIR},
		  {AIR,		 AIR, OAK_WOOD}},			{OAK_PLANK, 4}},

		{{{OAK_PLANK,	     AIR, AIR},
		  {OAK_PLANK,	     AIR, AIR},
		  {AIR,		 AIR, AIR}},				{STICK, 2}},

		{{{AIR,	     OAK_PLANK, AIR},
		  {AIR,	     OAK_PLANK, AIR},
		  {AIR,		 AIR, AIR}},				{STICK, 2}},

		{{{AIR,	     AIR, OAK_PLANK},
		  {AIR,	     AIR, OAK_PLANK},
		  {AIR,		 AIR, AIR}},				{STICK, 2}},

		{{{AIR,	     AIR, AIR},
		  {OAK_PLANK,	     AIR, AIR},
		  {OAK_PLANK,		 AIR, AIR}},		{STICK, 2}},

		{{{AIR,	     AIR, AIR},
		  {AIR,	     OAK_PLANK, AIR},
		  {AIR,		 OAK_PLANK, AIR}},			{STICK, 2}},

		{{{AIR,	     AIR, AIR},
		  {AIR,	     AIR, OAK_PLANK},
		  {AIR,		 AIR, OAK_PLANK}},			{STICK, 2}},

		{{{OAK_PLANK, OAK_PLANK, AIR},
		  {OAK_PLANK, OAK_PLANK, AIR},
		  {AIR,		 AIR, AIR}},				{CRAFTING_TABLE, 1}},

		{{{AIR,	     AIR, AIR},
		  {OAK_PLANK,	     OAK_PLANK, AIR},
		  {OAK_PLANK,		 OAK_PLANK, AIR}},	{CRAFTING_TABLE, 1}},

		{{{AIR,	     AIR, AIR},
		  {AIR,	     OAK_PLANK, OAK_PLANK},
		  {AIR,		 OAK_PLANK, OAK_PLANK}},	{CRAFTING_TABLE, 1}},

		{{{AIR,	     OAK_PLANK, OAK_PLANK},
		  {AIR,	     OAK_PLANK, OAK_PLANK},
		  {AIR,		 AIR, AIR}},				{CRAFTING_TABLE, 1}},

		{{{OAK_PLANK, OAK_PLANK, OAK_PLANK},
		  {AIR,	      STICK,	 AIR},
		  {AIR,	      STICK,	 AIR}},			{WOODEN_PICKAXE, 1}},

		{{{OAK_PLANK, OAK_PLANK, AIR},
		  {OAK_PLANK, STICK,	 AIR},
		  {AIR,	      STICK,	 AIR}},			{WOODEN_AXE, 1}},

		{{{AIR,       OAK_PLANK,  OAK_PLANK},
		  {AIR,	      STICK,	  OAK_PLANK},
		  {AIR,	      STICK,	  AIR}},		{WOODEN_AXE, 1}}
	};

	//json jrecipe;
public:
	Recipe() {
		//ifstream ifs(recipespath.c_str());
		//if (!ifs) {
		//	cout << recipespath << " not found!" << endl;
		//	return;
		//}
		//cout << "parsing " << recipespath.c_str() << endl;
		//ifs >> jrecipe;
	}

	ItemResult getRecipe(InventorySlot slots[3][3]);

	ItemResult getRecipe(InventorySlot slots[2][2]);

	bool itemPlaceable(Item itemType);

	bool itemUsable(Item itemType);

	bool isTool(Item itemType);

	bool isBreakable(Item itemType);
};

Recipe recipe;

ItemResult Recipe::getRecipe(InventorySlot slots[3][3]) {
	for (int i = 0; i < recipes.size(); i++) {
		if (slots[0][0].item == recipes[i].grid[0][0] && slots[0][1].item == recipes[i].grid[0][1] && slots[0][2].item == recipes[i].grid[0][2] &&
			slots[1][0].item == recipes[i].grid[1][0] && slots[1][1].item == recipes[i].grid[1][1] && slots[1][2].item == recipes[i].grid[1][2] &&
			slots[2][0].item == recipes[i].grid[2][0] && slots[2][1].item == recipes[i].grid[2][1] && slots[2][2].item == recipes[i].grid[2][2]) {
			return recipes[i].result;
		}
	}
	return { AIR, 0 };
}

ItemResult Recipe::getRecipe(InventorySlot slots[2][2]) {
	for (int i = 0; i < recipes.size(); i++) {
		if (slots[0][0].item == recipes[i].grid[0][0] && slots[0][1].item == recipes[i].grid[0][1] && recipes[i].grid[0][2] == AIR &&
			slots[1][0].item == recipes[i].grid[1][0] && slots[1][1].item == recipes[i].grid[1][1] && recipes[i].grid[1][2] == AIR &&
			recipes[i].grid[2][0] == AIR			  && recipes[i].grid[2][1] == AIR			   && recipes[i].grid[2][2] == AIR) {
			return recipes[i].result;
		}
	}
	return { AIR, 0 };
}

//Item Recipe::getRecipe(InventorySlot slots[3][3]) {
//	for (int i = 0; i < bigrecipes.size(); i += 10) {
//		if (slots[0][0].item == bigrecipes[i + 0] && slots[0][1].item == bigrecipes[i + 1] && slots[0][2].item == bigrecipes[i + 2] &&
//			slots[1][0].item == bigrecipes[i + 3] && slots[1][1].item == bigrecipes[i + 4] && slots[1][2].item == bigrecipes[i + 5] &&
//			slots[2][0].item == bigrecipes[i + 6] && slots[2][1].item == bigrecipes[i + 7] && slots[2][2].item == bigrecipes[i + 8]) {
//			return bigrecipes[i + 9];
//		}
//	}
//	return AIR;
//}
//
//Item Recipe::getRecipe(InventorySlot slots[2][2]) {
//	vector<string> slots_string;
//	for (int i = 0; i < 2; i++) {
//		for(int j = 0; j < 2; j++){
//			slots_string.push_back(itemTypeString[slots[i][j].item.id]);
//		}
//	}
//	
//	for (auto itrecipe = jrecipe["recipes"].begin(); itrecipe < jrecipe["recipes"].end(); itrecipe++) {
//		vector<string> ingredients = (*itrecipe)["ingredients"].get<vector<string>>();
//		bool containsAll = true;
//		for (string ingr : ingredients) {
//			if (ingr != "air" && !contains(slots_string.data(), slots_string.size(), ingr)) {
//				containsAll = false;
//				break;
//			}
//		}
//		if (!containsAll) continue;
//		vector<pair<string, string>> recipe_symbols = (*itrecipe)["pattern"]["symbols"].get<vector<pair<string, string>>>();
//		for (auto itgrid = (*itrecipe)["pattern"]["grid"].begin(); itgrid < (*itgrid)["pattern"]["grid"].end(); itgrid++) {
//			vector<string> recipe_pattern = (*itgrid).get<vector<string>>();
//			for (int i = 0; i < recipe_symbols.size(); i++) {
//				replace(recipe_pattern.begin(), recipe_pattern.end(), recipe_symbols[i].first, recipe_symbols[i].second);
//			}
//
//		}
//		
//	}
//	return AIR;
//}

bool Recipe::itemPlaceable(Item itemType) {
	return (itemType.isPlaceable());
}

bool Recipe::itemUsable(Item itemType) {
	return (itemType.isUsable());
}

bool Recipe::isTool(Item itemType) {
	return (itemType.isTool());
}

bool Recipe::isBreakable(Item itemType) {
	return (itemType.isBreakable());
}