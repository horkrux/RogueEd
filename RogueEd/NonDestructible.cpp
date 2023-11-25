#include "NonDestructible.h"

HOB* NonDestructible::m_referenceHOB = NULL;
int NonDestructible::referenceCount = 0;

NonDestructible::NonDestructible(ID3D11Device1* device, ID3D11DeviceContext1* deviceContext, char levelId)
{
	if (!m_referenceHOB)
	{
		m_referenceHOB = new HOB();

		vector<char> hobContents;
		vector<char> hmtContents;

		DataIO* dataIO = DataIO::getInstance();
		char path[32];
		sprintf_s(path, 32, "data/level/lv_%c/opkg_HOB", levelId);
		dataIO->getFileContents(path, &hobContents);
		path[22] = 'M';
		path[23] = 'T';
		dataIO->getFileContents(path, &hmtContents);

		m_referenceHOB->initializeSHIT(device, deviceContext, hobContents, hmtContents);
	}

	referenceCount++;
}

NonDestructible::NonDestructible(const NonDestructible& cpy) {}

NonDestructible::~NonDestructible()
{
	referenceCount--;

	if (!referenceCount)
	{
		delete m_referenceHOB;
		m_referenceHOB = NULL;
	}
}

RogueObjectType NonDestructible::getType() const
{
	return NON_DESTRUCTIBLE;
}

int NonDestructible::read(char* pObj)
{
	memcpy(&m_params, pObj, sizeof(NonDestructible::Params));

	m_name = pObj + m_params.offsetName;
	m_eventId = m_params.index;
	m_position = m_params.position;
	m_rotation = m_params.rotation;
	m_scale = m_params.scale;
	m_modelName = pObj + m_params.offsetModelName;
	m_modelId = -1;

	for (int i = 0; i < m_referenceHOB->getNumObjects(); i++)
	{
		if (!strcmp(m_modelName.c_str(), m_referenceHOB->getObjects()[i].name))
		{
			m_modelId = i;
			break;
		}
	}

	if (m_modelId == -1) return 1;

	m_worldMatrix = XMMatrixRotationRollPitchYaw(m_rotation.x * 0.0174532925f, m_rotation.y * 0.0174532925f, m_rotation.z * 0.0174532925f);
	//m_worldMatrix = XMMatrixTranslation(-m_position.x, m_position.y, -m_position.z);
	m_worldMatrix *= XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	m_worldMatrix *= XMMatrixTranslation(m_position.x, -m_position.y, m_position.z);
	//m_worldMatrix *= XMMatrixReflect(XMVectorSet(m_position.x, -m_position.y, m_position.z, 1));
	//m_worldMatrix *= XMMatrixTranslation(m_position.x, -m_position.y, m_position.z);
	//XMMatrixI

	return 0;
}

HOB* NonDestructible::getHobReference()
{
	return m_referenceHOB;
}

void NonDestructible::setHobReference(HOB* hob)
{
	m_referenceHOB = hob;
}