#include "ProbeDroid.h"

HOB* ProbeDroid::m_referenceHOB = NULL;
int ProbeDroid::referenceCount = 0;

ProbeDroid::ProbeDroid(ID3D11Device1* device, ID3D11DeviceContext1* deviceContext) {
	if (!m_referenceHOB)
	{
		m_referenceHOB = new HOB();

		vector<char> hobContents;
		vector<char> hmtContents;

		DataIO* dataIO = DataIO::getInstance();
		dataIO->getFileContents("data/imp_stuff/prbdroid_HOB", &hobContents);
		dataIO->getFileContents("data/imp_stuff/prbdroid_HMT", &hmtContents);

		m_referenceHOB->initializeSHIT(device, deviceContext, hobContents, hmtContents);
	}

	referenceCount++;
}

ProbeDroid::ProbeDroid(const ProbeDroid& cpy) {}

ProbeDroid::~ProbeDroid()
{
	referenceCount--;

	if (!referenceCount)
	{
		delete m_referenceHOB;
		m_referenceHOB = NULL;
	}
}

RogueObjectType ProbeDroid::getType() const
{
	return PROBE_DROID;
}

int ProbeDroid::read(char* pObj)
{
	memcpy(&m_params, pObj, sizeof(ProbeDroid::Params));

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

HOB* ProbeDroid::getHobReference()
{
	return m_referenceHOB;
}

void ProbeDroid::setHobReference(HOB* hob)
{
	m_referenceHOB = hob;
}