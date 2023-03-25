#include "CartoonConstantBuffer.h"

CCartoonConstantBuffer::CCartoonConstantBuffer() : m_BufferData{}
{
}

CCartoonConstantBuffer::CCartoonConstantBuffer(const CCartoonConstantBuffer& Buffer)
{
	m_BufferData = Buffer.m_BufferData;
}

CCartoonConstantBuffer::~CCartoonConstantBuffer()
{
}

bool CCartoonConstantBuffer::Init()
{
	SetConstantBuffer("Cartoon");/*

	m_BufferData.WorldViewProj = DirectX::XMMatrixTranspose(worldViewProj);
	m_BufferData.World = DirectX::XMMatrixTranspose(world);
	m_BufferData.View = DirectX::XMMatrixTranspose(view);
	m_BufferData.InvView = DirectX::XMMatrixTranspose(invView);
	m_BufferData.LightDir = DirectX::XMFLOAT4(lightDir.x, lightDir.y, lightDir.z, 0.0f);
	m_BufferData.ObjectColor = DirectX::XMFLOAT4(objectColor.x, objectColor.y, objectColor.z, 1.0f);*/

	return true;
}

void CCartoonConstantBuffer::UpdateBuffer()
{
	m_Buffer->UpdateBuffer(&m_BufferData);
}

CCartoonConstantBuffer* CCartoonConstantBuffer::Clone()
{
	return new CCartoonConstantBuffer(*this);
}
