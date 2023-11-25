#pragma once

#include "Level.h"
#include "RogueObject.h"
#include "ATST.h"
#include "ATAT.h"
#include "MegaTurret.h"
#include "GunTurret.h"
#include "NonDestructible.h"
#include "GenericDestroyable.h"
#include "RebTrans.h"
#include "RebCombat.h"
#include "ProbeDroid.h"
#include "TIE_Bomber.h"
#include "Civilian.h"
#include "Stormtrooper.h"
#include "ModelCommonTypes.h"
#include "DbgShapeBox.h"
#include "DbgShapeSphere.h"
#include "Spline.h"

#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

class LevelIO
{
public:
	static LevelIO* getInstance();

	int readDat(ID3D11Device1* device, ID3D11DeviceContext1* deviceContext, Level*, vector<char>*);
	int readHmp(Level*, vector<char>*);
	int readTEXT(ID3D11Device1*, ID3D11DeviceContext1*, Level*, vector<char>*, vector<char>*);

	int writeHmpToBitmap(const char*, vector<char>*);

private:
	LevelIO();
	LevelIO(const LevelIO&);
	~LevelIO();

	static LevelIO* single;
};