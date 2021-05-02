#include "test.h"

ReconstructionLUT::ReconstructionLUT(int Height, int Width) : m_Height(Height), m_Width(Width)
{
	this->m_Xpe = NewSpace(m_Height, m_Width);
	this->m_JXw = NewSpace(m_Height, m_Width);
	this->m_JYw = NewSpace(m_Height, m_Width);
	this->m_JZw = NewSpace(m_Height, m_Width);
	std::cout << "构造函数调用成功" << std::endl;
}
ReconstructionLUT::ReconstructionLUT(double **CameraM, double **ProjectM, int Height, int Width) : m_Height(Height), m_Width(Width), m_CameraM(CameraM), m_ProjectM(ProjectM)
{
	this->m_Xpe = NewSpace(m_Height, m_Width);
	this->m_JXw = NewSpace(m_Height, m_Width);
	this->m_JYw = NewSpace(m_Height, m_Width);
	this->m_JZw = NewSpace(m_Height, m_Width);
	std::cout << "构造函数调用成功" << std::endl;
}

ReconstructionLUT::~ReconstructionLUT()
{
	DeleteSpace(this->m_Xpe, m_Height, m_Width);
	DeleteSpace(this->m_JXw, m_Height, m_Width);
	DeleteSpace(this->m_JYw, m_Height, m_Width);
	DeleteSpace(this->m_JZw, m_Height, m_Width);

	std::cout << "析造函数调用成功" << std::endl;
}

void ReconstructionLUT::CalculationLUT()
{
}

void ReconstructionLUT::CalculationOpticalCenter()
{
	double m11 = m_CameraM[0][0];
	double m12 = m_CameraM[0][1];
	double m13 = m_CameraM[0][2];
	double m14 = m_CameraM[0][3];

	double m21 = m_CameraM[1][0];
	double m22 = m_CameraM[1][1];
	double m23 = m_CameraM[1][2];
	double m24 = m_CameraM[1][3];

	double m31 = m_CameraM[2][0];
	double m32 = m_CameraM[2][1];
	double m33 = m_CameraM[2][2];
	double m34 = m_CameraM[2][3];

	double T = m12 * (m21 * m33 - m23 * m31) - m11 * (m22 * m33 - m23 * m32) - m13 * (m21 * m32 - m22 * m31);
	m_Xoc = (m14 * (m22 * m33 - m23 * m32) + m24 * (m13 * m32 - m12 * m33) + m34 * (m12 * m23 - m13 * m22)) / T;
	m_Yoc = (m14 * (m23 * m31 - m21 * m33) + m24 * (m11 * m33 - m13 * m31) + m34 * (m13 * m21 - m11 * m23)) / T;
	m_Zoc = (m14 * (m21 * m32 - m22 * m31) + m24 * (m12 * m31 - m11 * m32) + m34 * (m11 * m22 - m12 * m21)) / T;
}
