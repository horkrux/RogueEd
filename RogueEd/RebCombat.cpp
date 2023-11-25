#include "RebCombat.h"

HOB* RebCombat::m_referenceHOB = NULL;
int RebCombat::referenceCount = 0;

RebCombat::RebCombat(ID3D11Device1* device, ID3D11DeviceContext1* deviceContext) {
	if (!m_referenceHOB)
	{
		m_referenceHOB = new HOB();

		vector<char> hobContents;
		vector<char> hmtContents;

		DataIO* dataIO = DataIO::getInstance();
		dataIO->getFileContents("data/reb_stuff/r_combat_HOB", &hobContents);
		dataIO->getFileContents("data/reb_stuff/r_combat_HMT", &hmtContents);

		m_referenceHOB->initializeSHIT(device, deviceContext, hobContents, hmtContents);
	}

	referenceCount++;
}

RebCombat::RebCombat(const RebCombat& cpy) {}

RebCombat::~RebCombat()
{
	referenceCount--;

	if (!referenceCount)
	{
		delete m_referenceHOB;
		m_referenceHOB = NULL;
	}
}

RogueObjectType RebCombat::getType() const
{
	return REB_COMBAT;
}

int RebCombat::read(char* pObj)
{
	memcpy(&m_params, pObj, sizeof(RebCombat::Params));

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

HOB* RebCombat::getHobReference()
{
	return m_referenceHOB;
}

void RebCombat::setHobReference(HOB* hob)
{
	m_referenceHOB = hob;
}