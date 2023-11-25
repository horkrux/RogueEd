#pragma once

#include "RogueObject.h"

class GenericDestroyable : RogueObject
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
	GenericDestroyable(ID3D11Device1*, ID3D11DeviceContext1*, char);
	GenericDestroyable(const GenericDestroyable&);
	virtual ~GenericDestroyable();

	virtual RogueObjectType getType() const;
	virtual int read(char*);

	virtual HOB* getHobReference();
	virtual void setHobReference(HOB*);

private:
	Params m_params;
	static HOB* m_referenceHOB;
	static int referenceCount;

	string m_modelName;
};