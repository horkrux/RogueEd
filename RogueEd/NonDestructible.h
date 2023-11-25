#pragma once

#include "RogueObject.h"

class NonDestructible : RogueObject
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
		int unk3[3];
		uint32_t offsetModelName;
		int unk4[3];
		int health;
		int unk6;
	} Params;

public:
	NonDestructible(ID3D11Device1*, ID3D11DeviceContext1*, char);
	NonDestructible(NonDestructible const&);
	~NonDestructible();

	virtual RogueObjectType getType() const;
	virtual int read(char*);

	virtual HOB* getHobReference();
	virtual void setHobReference(HOB*);

private:
	Params m_params;

	string m_modelName;

	static HOB* m_referenceHOB;
	static int referenceCount;
};
