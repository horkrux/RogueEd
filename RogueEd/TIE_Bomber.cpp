#include "TIE_Bomber.h"

HOB* TIE_Bomber::m_referenceHOB = NULL;
int TIE_Bomber::referenceCount = 0;

TIE_Bomber::TIE_Bomber(ID3D11Device1* device, ID3D11DeviceContext1* deviceContext) {
	if (!m_referenceHOB)
	{
		m_referenceHOB = new HOB();

		vector<char> hobContents;
		vector<char> hmtContents;

		DataIO* dataIO = DataIO::getInstance();
		dataIO->getFileContents("data/imp_stuff/tiebomber_HOB", &hobContents);
		dataIO->getFileContents("data/imp_stuff/tiebomber_HMT", &hmtContents);

		m_referenceHOB->initializeSHIT(device, deviceContext, hobContents, hmtContents);
	}

	referenceCount++;
}

TIE_Bomber::TIE_Bomber(const TIE_Bomber& cpy) {}

TIE_Bomber::~TIE_Bomber()
{
	referenceCount--;

	if (!referenceCount)
	{
		delete m_referenceHOB;
		m_referenceHOB = NULL;
	}
}

RogueObjectType TIE_Bomber::getType() const
{
	return TIE_BOMBER;
}

int TIE_Bomber::read(char* pObj)
{
	memcpy(&m_params, pObj, sizeof(TIE_Bomber::Params));

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

HOB* TIE_Bomber::getHobReference()
{
	return m_referenceHOB;
}

void TIE_Bomber::setHobReference(HOB* hob)
{
	m_referenceHOB = hob;
}