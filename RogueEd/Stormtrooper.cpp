#include "Stormtrooper.h"

HOB* Stormtrooper::m_referenceHOB = NULL;
int Stormtrooper::referenceCount = 0;

Stormtrooper::Stormtrooper(ID3D11Device1* device, ID3D11DeviceContext1* deviceContext) {
	if (!m_referenceHOB)
	{
		m_referenceHOB = new HOB();

		vector<char> hobContents;
		vector<char> hmtContents;

		DataIO* dataIO = DataIO::getInstance();
		dataIO->getFileContents("data/imp_stuff/trooper_HOB", &hobContents);
		dataIO->getFileContents("data/imp_stuff/trooper_HMT", &hmtContents);

		m_referenceHOB->initializeSHIT(device, deviceContext, hobContents, hmtContents);
	}

	referenceCount++;
}

Stormtrooper::Stormtrooper(const Stormtrooper& cpy) {}

Stormtrooper::~Stormtrooper()
{
	referenceCount--;

	if (!referenceCount)
	{
		delete m_referenceHOB;
		m_referenceHOB = NULL;
	}
}

RogueObjectType Stormtrooper::getType() const
{
	return STORMTROOPER;
}

int Stormtrooper::read(char* pObj)
{
	memcpy(&m_params, pObj, sizeof(Stormtrooper::Params));

	m_name = pObj + m_params.offsetName;
	m_eventId = m_params.index;
	m_position = m_params.position;
	m_rotation = m_params.rotation;
	m_scale = m_params.scale;
	m_health = m_params.health;

	m_worldMatrix = XMMatrixRotationRollPitchYaw(m_rotation.x * 0.0174532925f, m_rotation.y * 0.0174532925f, m_rotation.z * 0.0174532925f);
	m_worldMatrix *= XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	m_worldMatrix *= XMMatrixTranslation(m_position.x, -m_position.y, m_position.z);

	//m_worldMatrix = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z) + XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z) + XMMatrixTranslation(m_position.x, m_position.y, m_position.z);

	return 0;
}

HOB* Stormtrooper::getHobReference()
{
	return m_referenceHOB;
}

void Stormtrooper::setHobReference(HOB* hob)
{
	m_referenceHOB = hob;
}
