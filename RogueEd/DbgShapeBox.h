#pragma once

#include "DbgShape.h"
#include "Box.h"

class DbgShapeBox : public DbgShape
{
public:
	DbgShapeBox(ID3D11Device1*, ID3D11DeviceContext1*);
	DbgShapeBox(const DbgShapeBox&);
	~DbgShapeBox();

	XMFLOAT3 getScale();
	void setScale(XMFLOAT3);
	XMFLOAT3 getRotation();
	void setRotation(XMFLOAT3);

	Box* getReferenceBox();

	virtual int read(char*);
	//void render(ID3D11Device1*, ID3D11DeviceContext1*);
protected:
	typedef struct params : ParamsBasic
	{
		XMFLOAT3 rotation;
		XMFLOAT3 scale;
	} Params;

	Params m_params; //this is useless and I need to remove it

	XMFLOAT3 m_rotation;
	XMFLOAT3 m_scale;

private:
	static Box* m_referenceBox;
	//static int referenceCount;
};