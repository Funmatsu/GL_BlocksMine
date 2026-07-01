#pragma once

#include "Text.h"
#include "Window.h"

std::queue<vec2> chunkUpdateRequestQueue;
std::mutex chunkUpdateRequestMutex;
std::condition_variable chunkUpdateCV;

//#define AIR				  0
//#define DIAMOND_ORE       1
//#define GRASS_BLOCK       2
//#define IRON_ORE          3
//#define STONE_BLOCK       4
//#define DIRT_BLOCK        5
//#define OAK_WOOD          6
//#define CLOUD             7
//#define OAK_LEAVES        8
//#define OAK_PLANK         9
//#define POPPY             10
//#define BLUE_ORCHID       11
//#define GRASS             12
//
//#define CRAFTING_TABLE    21
////This is the limit for checking if a block is a tool or breakable; Dont change it's position!(Dont change the positions of crafting table and bedrock
//#define BEDROCK           100
//
//
//#define WOODEN_PICKAXE 101
//#define STICK 102

uint8 attrs[19] = { // block attributes : {bool unculled | bool placeable | bool useable | bool toolable | bool breakable | bool flatable | int luminable}
	0x44,
	0x24,
	0x24,
	0x24,
	0x24,
	0x24,
	0x24,
	0x24,
	0x64,
	0x24,
	0x66,
	0x66,
	0x66,
	0x34,
	0x20,
	0x67,
	0x2E,
	0x2E,
	0x2E
}; // 0x0111 1111

class Item {
private:
public:
	uint8_t id : 5;

	Item() { id = 0; }
	Item(int itemid) { id = itemid; }
	~Item() { id = 0; }

	bool isUncullable()			{ return ((attrs[id] >> 6) & 1); }

	bool isPlaceable()			{ return ((attrs[id] >> 5) & 1); }

	bool isUsable()				{ return ((attrs[id] >> 4) & 1); }

	bool isTool()				{ return ((attrs[id] >> 3) & 1); }

	bool isBreakable()			{ return ((attrs[id] >> 2) & 1); }

	bool isFlat()				{ return ((attrs[id] >> 1) & 1); }

	bool isLuninous()			{ return ((attrs[id] >> 0) & 1); }     

	void assignLight(PointLight* pLight, vec3 position) {		
		if (isLuninous()) {
			pLight[pointLightCount] = PointLight(1.0f, 0.9f, 0.5f,
				1.0f, 0.2f,
				position.x, position.y, position.z,
				0.2, 0.1f, 0.05f);
			pointLightCount++;
		}
	} 

	void deassignLight(PointLight* pLight, vec3 position) {
		pLight[pointLightCount] = PointLight();
		if (isLuninous() && pointLightCount > 0) {
			pointLightCount--;
		}
	}

	Item(const Item& item)				{ id = item.id; }

	void operator=(Item item)			{ id = item.id; }

	bool operator==(Item item)			{ return(id == item.id); }

	bool operator!=(Item item)			{ return(id != item.id); }

	friend ostream& operator<<(ostream& os, Item& item);

	Item(int item_id, bool placeable, bool useable, bool toolable, bool breakable, bool flatable, int luminable) { id = item_id; }
};

ostream& operator<<(ostream& os, Item& item) {
	os << item.id;
	return os;
}

Item AIR				( 0);
Item DIAMOND_ORE		( 1);
Item GRASS_BLOCK		( 2);
Item IRON_ORE			( 3);
Item STONE_BLOCK		( 4);
Item DIRT_BLOCK			( 5);
Item OAK_WOOD			( 6);
Item CLOUD				( 7);
Item OAK_LEAVES			( 8);
Item OAK_PLANK			( 9);
Item POPPY				(10);
Item BLUE_ORCHID		(11);
Item GRASS				(12);
Item CRAFTING_TABLE		(13);
Item BEDROCK			(14);
Item TORCH				(15);
Item STICK				(16);
Item WOODEN_PICKAXE		(17);
Item WOODEN_AXE			(18);

Item items[] = {
	AIR,
	DIAMOND_ORE,
	GRASS_BLOCK,
	IRON_ORE,
	STONE_BLOCK,
	DIRT_BLOCK,
	OAK_WOOD,
	CLOUD,
	OAK_LEAVES,
	OAK_PLANK,
	POPPY,
	BLUE_ORCHID,
	GRASS,
	CRAFTING_TABLE,
	BEDROCK,
	TORCH,
	STICK,
	WOODEN_PICKAXE,
	WOODEN_AXE
};

vector<string> itemTypeString{
	"air",
	"diamond_ore",
	"grass_block",
	"iron_ore",
	"stone_block",
	"dirt_block",
	"oak_wood",
	"cloud",
	"oak_leaves",
	"oak_plank",
	"poppy",
	"blue_orchid",
	"grass",
	"crafting_table",
	"bedrock",
	"torch",
	"stick",
	"wooden_pickaxe",
	"wooden_axe"
};

Item item(uint8_t idx) {
	return items[idx];
}

bool isFlat(uint8_t id) {
	return id < TORCH.id && items[id].isFlat();
}

bool isUncullable(uint8_t id) {
	return id < TORCH.id && items[id].isUncullable();
}

float uvTable[16][7] {
	{1, 0, 0, 0, 0, 0, 1   },
	{4, 4, 0, 0, 0, 0, 1   },
	{3, 2,-3,-2,-3, 1, 1   },
	{1, 3, 0, 0, 0, 0, 1   },
	{0, 1, 0, 0, 0, 0, 1   },
	{0, 3, 0, 0, 0, 0, 1   },
	{0, 2, 2, 1, 2, 1, 1   },
	{4, 2, 0, 0, 0, 0, 0.75},
	{4, 3, 0, 0, 0, 0, 1   },
	{3, 3, 0, 0, 0, 0, 1   },
	{2, 2, 0, 0, 0, 0, 1   },
	{1, 4, 0, 0, 0, 0, 1   },
	{1, 2, 0, 0, 0, 0, 1   },
	{2, 4, 1, 0, 1,-1, 1   },
	{0, 4, 0, 0, 0, 0, 1   },
	{1, 1, 0, 0, 0, 0, 1   },
};

float* getUVs(Item blockType) {
	return uvTable[blockType.id];
}
float* getUVs(int blockType) {
	return uvTable[blockType];
}
void getUVs(Item blockType, float* attrs) {
	for (int i = 0; i < 7; i++) {
		attrs[i] = uvTable[blockType.id][i];
	}
}
void getUVs(int blockType, float* attrs) {
	for (int i = 0; i < 7; i++) {
		attrs[i] = uvTable[blockType][i];
	}
	//memcpy(attrs, uvTable[blockType], 7 * sizeof(float));
}
void getUVs(Item blockType, float* attrs, int du) {
	attrs[0] = 1, attrs[1] = 0, attrs[2] = 0, attrs[3] = 0, attrs[4] = 0, attrs[5] = 0, attrs[6] = 1.0f;

	if (blockType == AIR) {
		attrs[0] = 1; attrs[1] = 0;
	}

	else if (blockType == DIAMOND_ORE) {
		attrs[0] = 4, attrs[1] = 4;
	}

	else if (blockType == GRASS_BLOCK) {
		attrs[0] = 3, attrs[1] = 2; attrs[2] = -3; attrs[3] = -2; attrs[4] = -3; attrs[5] = 1;
	}

	else if (blockType == IRON_ORE) {
		attrs[0] = 1; attrs[1] = 3;
	}

	else if (blockType == STONE_BLOCK) {
		attrs[0] = 0; attrs[1] = 1;
	}

	else if (blockType == DIRT_BLOCK) {
		attrs[0] = 0; attrs[1] = 3;
	}

	else if (blockType == OAK_WOOD) {
		attrs[0] = 0, attrs[1] = 2; attrs[2] = 2; attrs[3] = 1; attrs[4] = 2; attrs[5] = 1;
	}

	else if (blockType == CLOUD) {
		attrs[0] = 4, attrs[1] = 2; attrs[6] = 0.75f;
	}

	else if (blockType == OAK_PLANK) {
		attrs[0] = 3, attrs[1] = 3;
	}

	else if (blockType == OAK_LEAVES) {
		attrs[0] = 4, attrs[1] = 3;
	}

	else if (blockType == GRASS) {
		attrs[0] = 1, attrs[1] = 2;
	}

	else if (blockType == POPPY) {
		attrs[0] = 2, attrs[1] = 2;
	}

	else if (blockType == BLUE_ORCHID) {
		attrs[0] = 1, attrs[1] = 4;
	}

	else if (blockType == BEDROCK) {
		attrs[0] = 0, attrs[1] = 4;
	}

	else if (blockType == CRAFTING_TABLE) {
		attrs[0] = 2, attrs[1] = 4; attrs[2] = 1; attrs[3] = 0; attrs[4] = 1; attrs[5] = -1;
	}

	else if (blockType == TORCH) {
		attrs[0] = 1, attrs[1] = 1;
	}
}

//-------------------------------------------------------------------------------------------------------

struct UIElement {
public:
	mat4 model = mat4(1.0f);
	LightMesh quadMesh;
	int x, y, w, h;
	virtual void onClick() {};
};

struct Cursor : UIElement {
public:
	LightMesh mesh;
	Item item = AIR;
	int count = 0;
	Text textCount;
	void updateCount(int num) {
		textCount = Text(num);
	}
	~Cursor() {
		mesh.clearMesh();	
	}
};


Cursor cursor;

struct InventorySlot : UIElement
{
public:
	float angle = 0;
	LightMesh mesh; 
	Item item = AIR;
	int count = 0;
	Text textCount;
	void updateCount(int num) {
		textCount = Text(num);
	}
	void operator=(InventorySlot slot) {
		model = slot.model;
		mesh = slot.mesh;
		item = slot.item;
	}

	void operator=(Cursor cursor) {
		mesh = cursor.mesh;
		item = cursor.item;
		//quadMesh = cursor.quadMesh;
		count = cursor.count;
		cursor.count = 0;
	}

	void onClick() override {
		if (cursor.item != AIR && (item == AIR || mesh == LightMesh())) {
			item = cursor.item;
			mesh = cursor.mesh;
			
			if (mainWindow.getKeys()[GLFW_KEY_RIGHT_SHIFT]) {
				count += cursor.count;
				cursor.count = 0;
			}
			else {
				count++;
				if (cursor.count)
					cursor.count--;
			}
			
			cout << "called" << endl;
		}
		else {
			if (cursor.item == AIR && item != AIR) {
				cursor.item = item;
				cursor.mesh = mesh;

				if(count > 0){
					if (mainWindow.getKeys()[GLFW_KEY_RIGHT_SHIFT]) {
						cursor.count += count;
						count = 0;
					}
					else {
						cursor.count++;
						if (count)
							count--;
					}
				}
				if (cursor.count <= 0) { cursor.mesh.giveMesh(); }
				cout << "called twice" << endl;
			}
			else {
				if (cursor.item == item && item != AIR) {
					if (mainWindow.getKeys()[GLFW_KEY_LEFT_SHIFT]) {
						cursor.count++;
						if (count)
							count--;
					}
					if (mainWindow.getKeys()[GLFW_KEY_RIGHT_SHIFT]) {
						cursor.count += count;
						count = 0;
					}
					else {
						count++;
						if (cursor.count)
							cursor.count--;
					}
					cout << "called similarly" << endl;
				}
			}
		}

		//if (cursor.count <= 0) { cursor.mesh.giveMesh(); }
	}
};

struct UIButton : UIElement {
	//LightMesh mesh;
	Text title;
	bool pressed;

	std::function<int(int)> mainclick;

	UIButton(string text){
		title.defineWord(text, vec3(1));
	}

	template<typename Func, typename... Args>
	UIButton(string text, Func f, Args... argms) {
		title.defineWord(text, vec3(1));
		mainclick = f;
	}

	void onClick() override {
		mainclick(0);
	}

	void draw(mat4 ortho) {
		title.drawText(ortho);
	}
};

unsigned int itemFbo, itemColorTex, itemDepthTex;
void initItemTextures() {
	glGenTextures(1, &itemColorTex);
	glBindTexture(GL_TEXTURE_2D, itemColorTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1920, 1059, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glGenRenderbuffers(1, &itemDepthTex);
	glBindRenderbuffer(GL_RENDERBUFFER, itemDepthTex);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1920, 1059);

	glGenFramebuffers(1, &itemFbo);
	glBindFramebuffer(GL_FRAMEBUFFER, itemFbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, itemColorTex, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, itemDepthTex);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Item FBO is incomplete!" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}