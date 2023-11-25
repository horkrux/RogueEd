#include "GenericDestroyable.h"

HOB* GenericDestroyable::m_referenceHOB = NULL;
int GenericDestroyable::referenceCount = 0;

GenericDestroyable::GenericDestroyable(ID3D11Device1* device, ID3D11DeviceContext1* deviceContext, char levelId) {
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

GenericDestroyable::GenericDestroyable(const GenericDestroyable& cpy) {}

GenericDestroyable::~GenericDestroyable()
{
	referenceCount--;

	if (!referenceCount)
	{
		delete m_referenceHOB;
		m_referenceHOB = NULL;
	}
}

RogueObjectType GenericDestroyable::getType() const
{
	return GENERIC_DESTROYABLE;
}

int GenericDestroyable::read(char* pObj)
{
	memcpy(&m_params, pObj, sizeof(GenericDestroyable::Params));

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

	m_worldMatrix = XMMatrixRotationRollPitchYaw(m_rotation.x * 0.0174532925f, -m_rotation.y * 0.0174532925f, m_rotation.z * 0.0174532925f);
	m_worldMatrix *= XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	m_worldMatrix *= XMMatrixTranslation(m_position.x, -m_position.y, m_position.z);

	return 0;
}

HOB* GenericDestroyable::getHobReference()
{
	return m_referenceHOB;
}

void GenericDestroyable::setHobReference(HOB* hob)
{
	m_referenceHOB = hob;
}