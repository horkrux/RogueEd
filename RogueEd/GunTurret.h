#pragma once

#include "RogueObject.h"

class GunTurret : RogueObject
{
	typedef struct {
		uint32_t type;
		uint32_t index;
		uint32_t size;
		uint32_t offsetName;
		XMFLOAT3 position;
		XMFLOAT3 rotation;
		XMFLOAT3 scale;
		int unk1[6];
		int unk2;
		int unk3[7];
		uint32_t health;
		int unk4[5];
		int unk5;
		int unk6;
		//there is more, but idk wtf that is
	} Params;

public:
	GunTurret(ID3D11Device1*, ID3D11DeviceContext1*);
	GunTurret(const GunTurret&);
	~GunTurret();

	virtual RogueObjectType getType() const;
	virtual int read(char*);

	virtual HOB* getHobReference();
	virtual void setHobReference(HOB*);

private:
	Params m_params;
	static HOB* m_referenceHOB;
	static int referenceCount;
};