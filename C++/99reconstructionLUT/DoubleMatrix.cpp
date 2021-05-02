#include "DoubleMatrix.h"

CFloatMatrix::CFloatMatrix()
{
	Initialize(1, 1, 0, false);
}
CFloatMatrix::CFloatMatrix(CFloatMatrix &amMatrix)
{
	m_iRows = amMatrix.Rows();
	m_iColumns = amMatrix.Columns();
	m_ppfData = new float *[m_iRows];
	for (int i = 0; i < m_iRows; i++)
	{
		m_ppfData[i] = new float[m_iColumns];
	}
	for (int iRow = 0; iRow < m_iRows; iRow++)
	{
		for (int iCol = 0; iCol < m_iColumns; iCol++)
		{
			m_ppfData[iRow][iCol] = amMatrix(iRow, iCol);
		}
	}
}
CFloatMatrix::CFloatMatrix(int aiRows, int aiColumns)
{
	Initialize(aiRows, aiColumns, 0, false);
}

CFloatMatrix::CFloatMatrix(string astrFileName)
{
	QFile file(astrFileName);
	file.open(QIODevice::ReadOnly);
	int iData;
	//	QByteArray bytes = file.readAll();
	//	m_pv_iRows = *( ( int* )bytes.data(  ) );
	//	m_pv_iColumns = *( ( int* )( bytes.data(  ) + sizeof( m_pv_iRows ) ) );

	file.read((char *)&iData, sizeof(iData));
	m_iRows = iData;
	file.read((char *)&iData, sizeof(iData));
	m_iColumns = iData;
	float fData;
	m_ppfData = new float *[m_iRows];
	for (int i = 0; i < m_iRows; i++)
	{
		m_ppfData[i] = new float[m_iColumns];
	}
	for (int iRow = 0; iRow < m_iRows; iRow++)
	{
		for (int iCol = 0; iCol < m_iColumns; iCol++)
		{
			file.read((char *)&fData, sizeof(fData));
			m_ppfData[iRow][iCol] = fData;
		}
	}
	file.close();
}

CFloatMatrix::CFloatMatrix(int aiDimension)
{
	Initialize(aiDimension, aiDimension, 0, false);
}

CFloatMatrix::CFloatMatrix(int aiRows, int aiColumns, float adInitValue)
{
	Initialize(aiRows, aiColumns, adInitValue, true);
}

int CFloatMatrix::Rows()
{
	return m_iRows;
}

int CFloatMatrix::Columns()
{
	return m_iColumns;
}

int CFloatMatrix::Length()
{
	return m_iColumns * m_iRows;
}

CFloatMatrix::~CFloatMatrix()
{
	Release();
}

float CFloatMatrix::Get(int aiRow, int aiColumn)
{
	return m_ppfData[aiRow][aiColumn];
}

float CFloatMatrix::Get(int aiCount)
{
	int iRow = aiCount / m_iColumns;
	int iCol = aiCount % m_iColumns;
	return m_ppfData[iRow][iCol];
}

void CFloatMatrix::Set(int aiRow, int aiColumn, float afValue)
{
	m_ppfData[aiRow][aiColumn] = afValue;
}

void CFloatMatrix::SetSub(int aiRowStart, int aiRowEnd, int aiColStart, int aiColEnd, CFloatMatrix amSubMat)
{
	if (aiRowStart < 0 || aiRowStart >= m_iRows || aiRowEnd < 0 || aiRowEnd >= m_iRows || aiColStart < 0 || aiColStart >= m_iColumns || aiColEnd < 0 || aiColEnd >= m_iColumns)
	{
		throw("Invalid number of rows or columns");
		return;
	}
	if (aiRowEnd < aiRowStart || aiColEnd < aiColStart)
	{
		throw("Invalid number of rows or columns");
		return;
	}
	int iHeight = aiRowEnd - aiRowStart + 1;
	int iWidth = aiColEnd - aiColStart + 1;
	if (iHeight != amSubMat.Rows() || iWidth != amSubMat.Columns())
	{
		throw("Invalid number of rows or columns");
		return;
	}
	for (int iRow = aiRowStart; iRow <= aiRowEnd; iRow++)
	{
		for (int iCol = aiColStart; iCol <= aiColEnd; iCol++)
		{
			m_ppfData[iRow][iCol] = amSubMat(iRow - aiRowStart, iCol - aiColStart);
		}
	}
}

CFloatMatrix CFloatMatrix::GetSub(int aiRowStart, int aiRowEnd, int aiColStart, int aiColEnd)
{
	if (aiRowStart < 0 || aiRowStart >= m_iRows || aiRowEnd < 0 || aiRowEnd >= m_iRows || aiColStart < 0 || aiColStart >= m_iColumns || aiColEnd < 0 || aiColEnd >= m_iColumns)
	{
		throw("Invalid number of rows or columns");
		return NULL;
	}
	if (aiRowEnd < aiRowStart || aiColEnd < aiColStart)
	{
		throw("Invalid number of rows or columns");
		return NULL;
	}
	int iHeight = aiRowEnd - aiRowStart + 1;
	int iWidth = aiColEnd - aiColStart + 1;

	CFloatMatrix mReturn(iHeight, iWidth);
	for (int iRow = aiRowStart; iRow <= aiRowEnd; iRow++)
	{
		for (int iCol = aiColStart; iCol <= aiColEnd; iCol++)
		{
			mReturn[iRow - aiRowStart][iCol - aiColStart] = m_ppfData[iRow][iCol];
		}
	}
	return mReturn;
}

void CFloatMatrix::Initialize(int aiRows, int aiColumns, float adInitValue, bool abInit)
{
	if ((aiRows <= 0) || (aiColumns <= 0))
	{
		throw("Invalid number of rows or columns ( Constructor )");
		m_iRows = 0;
		m_iColumns = 0;
		return;
	}

	m_iRows = aiRows;
	m_iColumns = aiColumns;
	m_ppfData = new float *[m_iRows];
	for (int i = 0; i < m_iRows; i++)
	{
		m_ppfData[i] = new float[m_iColumns];
	}
	if (!abInit)
	{
		return;
	}
	Assign(adInitValue);
}

void CFloatMatrix::Initialize(CFloatMatrix &amMatrix)
{
	m_iRows = amMatrix.Rows();
	m_iColumns = amMatrix.Columns();

	m_ppfData = new float *[m_iRows];
	for (int iCount = 0; iCount < m_iRows; iCount++)
	{
		m_ppfData[iCount] = new float[m_iColumns];
	}

	for (int iRow = 0; iRow < m_iRows; iRow++)
	{
		for (int iCol = 0; iCol < m_iColumns; iCol++)
		{
			m_ppfData[iRow][iCol] = amMatrix(iRow, iCol);
		}
	}
}

void CFloatMatrix::Release(void)
{
	for (int iCount = 0; iCount < m_iRows; iCount++)
	{
		delete[] m_ppfData[iCount];
	}
	delete[] m_ppfData;
	m_ppfData = NULL;

	m_iRows = 0;
	m_iColumns = 0;
}

void CFloatMatrix::Assign(float adValue)
{
	for (int iRow = 0; iRow < m_iRows; iRow++)
	{
		for (int iCol = 0; iCol < m_iColumns; iCol++)
		{
			m_ppfData[iRow][iCol] = adValue;
		}
	}
}

void CFloatMatrix::Resize(int aiRows, int aiCols)
{
	Release();
	Initialize(aiRows, aiCols, 0, false);
}

void CFloatMatrix::Resize(int aiRows, int aiCols, CFloatMatrix amMatrix)
{
	if ((aiRows != amMatrix.Rows()) || (aiCols != amMatrix.Columns()))
	{
		throw("Invalid number of rows or columns ( Constructor )");
		return;
	}
	Release();
	Initialize(aiRows, aiCols, 0, false);
	for (int iRow = 0; iRow < aiRows; iRow++)
	{
		for (int iCol = 0; iCol < aiCols; iCol++)
		{
			m_ppfData[iRow][iCol] = amMatrix(iRow, iCol);
		}
	}
}

void CFloatMatrix::Resize(int aiRows, int aiCols, float adInitValue)
{
	Release();
	Initialize(aiRows, aiCols, adInitValue, true);
}

void CFloatMatrix::SaveAsBinary(string astrFileName)
{
	CFloatMatrix::SaveAsBinary(*this, astrFileName);
}

void CFloatMatrix::SaveAsBinary(CFloatMatrix amData, string astrFileName)
{
	QFile file(astrFileName);
	file.open(QIODevice::WriteOnly);

	int iRows = amData.Rows();
	int iCols = amData.Columns();

	file.write((char *)&iRows, sizeof(iRows));
	file.write((char *)&iCols, sizeof(iCols));
	for (int iRow = 0; iRow < iRows; iRow++)
	{
		for (int iCol = 0; iCol < iCols; iCol++)
		{
			float fTemp = amData(iRow, iCol);
			file.write((char *)&fTemp, sizeof(fTemp));
		}
	}
	file.close();
}

float *CFloatMatrix::operator[](int aiCount)
{
	return (m_ppfData[aiCount]);
}

float &CFloatMatrix::operator()(int aiRow, int aiCol)
{
	return (m_ppfData[aiRow][aiCol]);
}

float &CFloatMatrix::operator()(int aiCount)
{
	int iRow = aiCount / m_iColumns;
	int iCol = aiCount % m_iColumns;
	return m_ppfData[iRow][iCol];
}

CFloatMatrix &CFloatMatrix::operator=(CFloatMatrix &amMatrix)
{
	if (amMatrix.Rows() != m_iRows || amMatrix.Columns() != m_iColumns)
	{
		Release();

		Initialize(amMatrix);
	}
	else
	{
		CopyFrom(amMatrix);
	}

	return *this;
}

CFloatMatrix CFloatMatrix::operator+(CFloatMatrix amMatrix)
{
	if ((amMatrix.Rows() != m_iRows) || (amMatrix.Columns() != m_iColumns))
	{
		return *this;
	}

	CFloatMatrix mReturn(m_iRows, m_iColumns);

	for (int iRow = 0; iRow < m_iRows; iRow++)
	{
		for (int iCol = 0; iCol < m_iColumns; iCol++)
		{
			mReturn(iRow, iCol) = m_ppfData[iRow][iCol] + amMatrix(iRow, iCol);
		}
	}

	return mReturn;
}

CFloatMatrix CFloatMatrix::operator+(double adScalor)
{
	CFloatMatrix mReturn(m_iRows, m_iColumns);

	for (int iRow = 0; iRow < m_iRows; iRow++)
	{
		for (int iCol = 0; iCol < m_iColumns; iCol++)
		{
			mReturn(iRow, iCol) = float(m_ppfData[iRow][iCol] + adScalor);
		}
	}

	return mReturn;
}

CFloatMatrix operator+(double adScalor, CFloatMatrix amMatrix)
{
	int iRows = amMatrix.Rows();
	int iCols = amMatrix.Columns();
	CFloatMatrix mReturn(iRows, iCols);

	for (int iRow = 0; iRow < iRows; iRow++)
	{
		for (int iCol = 0; iCol < iCols; iCol++)
		{
			mReturn(iRow, iCol) = float(amMatrix(iRow, iCol) + adScalor);
		}
	}

	return mReturn;
}

CFloatMatrix CFloatMatrix::operator-(CFloatMatrix amMatrix)
{
	if ((amMatrix.Rows() != m_iRows) || (amMatrix.Columns() != m_iColumns))
	{
		return *this;
	}

	CFloatMatrix mReturn(m_iRows, m_iColumns);

	for (int iRow = 0; iRow < m_iRows; iRow++)
	{
		for (int iCol = 0; iCol < m_iColumns; iCol++)
		{
			mReturn(iRow, iCol) = m_ppfData[iRow][iCol] - amMatrix(iRow, iCol);
		}
	}

	return mReturn;
}

CFloatMatrix CFloatMatrix::operator-(double adScalor)
{
	CFloatMatrix mReturn(m_iRows, m_iColumns);

	for (int iRow = 0; iRow < m_iRows; iRow++)
	{
		for (int iCol = 0; iCol < m_iColumns; iCol++)
		{
			mReturn(iRow, iCol) = float(m_ppfData[iRow][iCol] - adScalor);
		}
	}

	return mReturn;
}

CFloatMatrix operator-(double adScalor, CFloatMatrix amMatrix)
{
	int iRows = amMatrix.Rows();
	int iCols = amMatrix.Columns();
	CFloatMatrix mReturn(iRows, iCols);

	for (int iRow = 0; iRow < iRows; iRow++)
	{
		for (int iCol = 0; iCol < iCols; iCol++)
		{
			mReturn(iRow, iCol) = float(adScalor - amMatrix(iRow, iCol));
		}
	}

	return mReturn;
}

CFloatMatrix CFloatMatrix::operator*(CFloatMatrix amMatrix)
{
	if (amMatrix.Rows() != m_iColumns)
	{
		return *this;
	}

	int iRows = m_iRows;
	int iCols = amMatrix.Columns();
	int iLength = m_iColumns;

	CFloatMatrix mReturn(iRows, iCols);

	for (int iRow = 0; iRow < iRows; iRow++)
	{
		for (int iCol = 0; iCol < iCols; iCol++)
		{
			mReturn(iRow, iCol) = 0;
			for (int iCount = 0; iCount < iLength; iCount++)
			{
				mReturn(iRow, iCol) += m_ppfData[iRow][iCount] * amMatrix(iCount, iCol);
			}
		}
	}

	return mReturn;
}

CFloatMatrix CFloatMatrix::operator*(double adScalor)
{
	CFloatMatrix mReturn(m_iRows, m_iColumns);

	for (int iRow = 0; iRow < m_iRows; iRow++)
	{
		for (int iCol = 0; iCol < m_iColumns; iCol++)
		{
			mReturn(iRow, iCol) = float(m_ppfData[iRow][iCol] * adScalor);
		}
	}

	return mReturn;
}

CFloatMatrix operator*(double adScalor, CFloatMatrix amMatrix)
{
	int iRows = amMatrix.Rows();
	int iCols = amMatrix.Columns();
	CFloatMatrix mReturn(iRows, iCols);

	for (int iRow = 0; iRow < iRows; iRow++)
	{
		for (int iCol = 0; iCol < iCols; iCol++)
		{
			mReturn(iRow, iCol) = float(adScalor * amMatrix[iRow][iCol]);
		}
	}

	return mReturn;
}

CFloatMatrix CFloatMatrix::operator/(double adScalar)
{
	CFloatMatrix mTemp(m_iRows, m_iColumns);
	for (int iRow = 0; iRow < m_iRows; iRow++)
	{
		for (int iCol = 0; iCol < m_iColumns; iCol++)
		{
			mTemp(iRow, iCol) = float(m_ppfData[iRow][iCol] / adScalar);
		}
	}

	return mTemp;
}

CFloatMatrix CFloatMatrix::Transpose(CFloatMatrix amMatrix)
{
	int iRows = amMatrix.Columns();
	int iCols = amMatrix.Rows();

	CFloatMatrix mTemp(iRows, iCols);

	for (int iRow = 0; iRow < iRows; iRow++)
	{
		for (int iCol = 0; iCol < iCols; iCol++)
		{
			mTemp(iRow, iCol) = amMatrix(iCol, iRow);
		}
	}

	return mTemp;
}

CFloatMatrix CFloatMatrix::Transpose(void)
{
	CFloatMatrix mTemp(m_iColumns, m_iRows);

	for (int iRow = 0; iRow < m_iRows; iRow++)
	{
		for (int iCol = 0; iCol < m_iColumns; iCol++)
		{
			mTemp(iCol, iRow) = m_ppfData[iRow][iCol];
		}
	}

	return mTemp;
}

CFloatMatrix CFloatMatrix::Copy(void)
{
	return *this;
}

void CFloatMatrix::CopyFrom(CFloatMatrix &amMatrix)
{
	int iRows = amMatrix.Rows();
	int iCols = amMatrix.Columns();

	for (int iRow = 0; iRow < iRows; iRow++)
	{
		for (int iCol = 0; iCol < iCols; iCol++)
		{
			m_ppfData[iRow][iCol] = amMatrix(iRow, iCol);
		}
	}
}

CFloatMatrix CFloatMatrix::LoadData(string astrFileName)
{
	QFile file(astrFileName);
	file.open(QIODevice::ReadOnly);
	int iData;
	file.read((char *)&iData, sizeof(iData));
	int iRows = iData;
	file.read((char *)&iData, sizeof(iData));
	int iCols = iData;
	CFloatMatrix mReturn(iRows, iCols);
	float fData;
	for (int iRow = 0; iRow < iRows; iRow++)
	{
		for (int iCol = 0; iCol < iCols; iCol++)
		{
			file.read((char *)&fData, sizeof(fData));
			mReturn(iRow, iCol) = fData;
		}
	}
	file.close();
	return mReturn;
}

CFloatMatrix CFloatMatrix::Identity(int aiDim)
{
	CFloatMatrix mTemp(aiDim, aiDim);

	for (int iRow = 0; iRow < aiDim; iRow++)
	{
		for (int iCol = 0; iCol < aiDim; iCol++)
		{
			if (iRow == iCol)
			{
				mTemp(iRow, iCol) = 1;
			}
			else
			{
				mTemp(iRow, iCol) = 0;
			}
		}
	}

	return mTemp;
}

float CFloatMatrix::VectorNorm(CFloatMatrix amVector, int aiNorm)
{
	if ((1 != amVector.Rows()) && (1 != amVector.Columns()))
	{
		throw("It should be a vector ( Norm )!");
		return -1;
	}

	//	float dNorm = aiNorm;
	int iLength = amVector.Rows() * amVector.Columns();

	if (aiNorm > 0)
	{
		float dSum = 0;

		for (int iCount = 0; iCount < iLength; iCount++)
		{
			dSum += (float)qPow(qAbs(amVector(iCount)), aiNorm);
		}
		return (float)qPow(dSum, 1.0 / aiNorm);
	}
	else // inf norm
	{
		float dMax = qAbs(amVector(0));
		float dTemp;
		for (int iCount = 1; iCount < iLength; iCount++)
		{
			dTemp = qAbs(amVector(iCount));
			if (dTemp > dMax)
			{
				dMax = dTemp;
			}
		}
		return (dMax);
	}
}

CFloatMatrix CFloatMatrix::ElementAcos(CFloatMatrix amA)
{
	int iRows = amA.Rows();
	int iCols = amA.Columns();

	CFloatMatrix amNewA(iRows, iCols, 0.0F);
	for (int iRow = 0; iRow < iRows; iRow++)
	{
		for (int iCol = 0; iCol < iCols; iCol++)
		{
			amNewA(iRow, iCol) = (float)qAcos(amA(iRow, iCol));
		}
	}
	return amNewA;
}
CFloatMatrix CFloatMatrix::ElementAtan2(CFloatMatrix amA, CFloatMatrix amB)
{
	if ((amA.Columns() != amB.Columns()) || (amA.Rows() != amB.Rows()))
	{
		throw("Error");
	}
	int iRows = amA.Rows();
	int iCols = amA.Columns();
	double pi = 3.141592653;
	CFloatMatrix amNewA(iRows, iCols, 0.0F);

	for (int iRow = 0; iRow < iRows; iRow++)
	{
		for (int iCol = 0; iCol < iCols; iCol++)
		{
			amNewA(iRow, iCol) = (float)(qAtan2(amA(iRow, iCol), amB(iRow, iCol)) + pi);
		}
	}
	return amNewA;
}

CFloatMatrix CFloatMatrix::ElementCos(CFloatMatrix amA)
{
	int iRows = amA.Rows();
	int iCols = amA.Columns();

	CFloatMatrix amNewA(iRows, iCols, 0.0F);
	for (int iRow = 0; iRow < iRows; iRow++)
	{
		for (int iCol = 0; iCol < iCols; iCol++)
		{
			amNewA(iRow, iCol) = (float)qCos(amA(iRow, iCol));
		}
	}
	return amNewA;
}

CFloatMatrix CFloatMatrix::ElementSin(CFloatMatrix amA)
{
	int iRows = amA.Rows();
	int iCols = amA.Columns();
	CFloatMatrix amNewA(iRows, iCols, 0.0F);
	for (int iRow = 0; iRow < iRows; iRow++)
	{
		for (int iCol = 0; iCol < iCols; iCol++)
		{
			float fSin = (float)qSin(double(amA(iRow, iCol)));
			amNewA(iRow, iCol) = fSin;
		}
	}
	return amNewA;
}

CFloatMatrix CFloatMatrix::ElementPower(CFloatMatrix amMatix, float adPower)
{
	int iRows = amMatix.Rows();
	int iCols = amMatix.Columns();
	CFloatMatrix mTemp(iRows, iCols);
	for (int iRow = 0; iRow < iRows; iRow++)
	{
		for (int iCol = 0; iCol < iCols; iCol++)
		{
			mTemp(iRow, iCol) = (float)qPow(amMatix(iRow, iCol), adPower);
		}
	}

	return mTemp;
}

float CFloatMatrix::Determinant3(CFloatMatrix amA)
{
	if ((3 != amA.Rows()) || (3 != amA.Columns()))
	{
		throw(" It should be a 3 x 3 matrix !");
		return 0;
	}

	float dDet = amA(0, 0) * amA(1, 1) * amA(2, 2);
	dDet += amA(1, 0) * amA(0, 2) * amA(2, 1);
	dDet += amA(2, 0) * amA(0, 1) * amA(1, 2);
	dDet -= amA(0, 0) * amA(1, 2) * amA(2, 1);
	dDet -= amA(1, 0) * amA(0, 1) * amA(2, 2);
	dDet -= amA(2, 0) * amA(0, 2) * amA(1, 1);

	return dDet;
}

CFloatMatrix CFloatMatrix::Inverse(CFloatMatrix &amMatrix)
{
	int iRows = amMatrix.Rows();
	int iCols = amMatrix.Columns();

	if (iRows != iCols)
	{
		throw("Invalid Dimension !");
		return NULL;
	}

	CFloatMatrix mA(iRows, iRows * 2);
	mA.SetSub(0, mA.Rows() - 1, 0, mA.Rows() - 1, amMatrix);
	mA.SetSub(0, mA.Rows() - 1, iRows, 2 * iRows - 1, CFloatMatrix::Identity(iRows));

	for (int iRow = 0; iRow < iRows; iRow++)
	{
		float dMax = qAbs(mA(iRow, iRow));
		int iLine = iRow;
		for (int iDiagCount = iRow + 1; iDiagCount < iRows; iDiagCount++)
		{
			float dTemp = qAbs(mA(iDiagCount, iRow));
			if (dTemp > dMax)
			{
				dMax = dTemp;
				iLine = iDiagCount;
			}
		}
		if (0 == dMax)
		{
			throw("Determinant is zero !");
			;
			return NULL;
		}
		if (iRow != iLine)
		{
			CFloatMatrix mTemp(mA.GetSub(iRow, iRow, 0, mA.Columns() - 1));
			mA.SetSub(iRow, iRow, 0, mA.Columns() - 1, mA.GetSub(iLine, iLine, 0, mA.Columns() - 1));
			mA.SetSub(iLine, iLine, 0, mA.Columns() - 1, mTemp);
		}

		mA.SetSub(iRow, iRow, 0, mA.Columns() - 1, mA.GetSub(iRow, iRow, 0, mA.Columns() - 1) / mA(iRow, iRow));
		for (int iCol = 0; iCol < iRows; iCol++)
		{
			if ((iRow == iCol) || (0 == mA(iCol, iRow)))
			{
				continue;
			}

			mA.SetSub(iCol, iCol, 0, mA.Columns() - 1, mA.GetSub(iCol, iCol, 0, mA.Columns() - 1) - mA.GetSub(iRow, iRow, 0, mA.Columns() - 1) * mA(iCol, iRow));
		}
	}

	CFloatMatrix mTemp(mA.GetSub(0, mA.Rows() - 1, iRows, 2 * iRows - 1));

	return mTemp;
}

CFloatMatrix CFloatMatrix::Inverse3(CFloatMatrix amA)
{
	if ((3 != amA.Rows()) || (3 != amA.Columns()))
	{
		throw(" It should be a 3 x 3 matrix !");
		return NULL;
	}

	float dDet = CFloatMatrix::Determinant3(amA);

	if (0 == dDet)
	{
		throw("Its determinant is zero !");
		return NULL;
	}

	CFloatMatrix mTemp(3, 3, 0.0F);

	mTemp(0, 0) = (amA(1, 1) * amA(2, 2) - amA(1, 2) * amA(2, 1)) / dDet;
	mTemp(0, 1) = (-amA(0, 1) * amA(2, 2) + amA(0, 2) * amA(2, 1)) / dDet;
	mTemp(0, 2) = (amA(0, 1) * amA(1, 2) - amA(0, 2) * amA(1, 1)) / dDet;
	mTemp(1, 0) = (-amA(1, 0) * amA(2, 2) + amA(1, 2) * amA(2, 0)) / dDet;
	mTemp(1, 1) = (amA(0, 0) * amA(2, 2) - amA(0, 2) * amA(2, 0)) / dDet;
	mTemp(1, 2) = (-amA(0, 0) * amA(1, 2) + amA(0, 2) * amA(1, 0)) / dDet;
	mTemp(2, 0) = (amA(1, 0) * amA(2, 1) - amA(1, 1) * amA(2, 0)) / dDet;
	mTemp(2, 1) = (-amA(0, 0) * amA(2, 1) + amA(0, 1) * amA(2, 0)) / dDet;
	mTemp(2, 2) = (amA(0, 0) * amA(1, 1) - amA(0, 1) * amA(1, 0)) / dDet;

	return mTemp;
}

CFloatMatrix CFloatMatrix::RemoveNaNWithIdx(CFloatMatrix amA, CFloatMatrix amIdx, int aiCount)
{
	int iCols = amA.Columns();
	int iRows = amA.Rows();
	int iCols2 = amIdx.Columns();
	int iRows2 = amIdx.Rows();
	if (0 == iCols && 0 == iRows && 0 == iCols2 && 0 == iRows2)
	{
		return NULL;
	}
	if (iCols != iCols2)
	{
		return NULL;
	}
	if (0 == aiCount)
	{
		return amA;
	}

	CFloatMatrix mNewA(iRows, aiCount, 0.0F);
	int iCnt = 0;

	for (int iC = 0; iC < iCols; iC++)
	{
		if (1.0F == amIdx(0, iC))
		{
			mNewA.SetSub(0, mNewA.Rows() - 1, iCnt, iCnt, amA.GetSub(0, mNewA.Rows() - 1, iC, iC));
			iCnt++;
		}
	}
	//    mNewA.SaveAsBinary( "mNewA.dat" );
	return mNewA;
}

float CFloatMatrix::Max(CFloatMatrix &amMatrix)
{
	float fMax = amMatrix(0, 0);

	int iRows = amMatrix.Rows();
	int iCols = amMatrix.Columns();

	float fTemp;

	for (int iRow = 0; iRow < iRows; iRow++)
	{
		for (int iCol = 0; iCol < iCols; iCol++)
		{
			fTemp = amMatrix(iRow, iCol);
			if ((fTemp == fTemp) && (fTemp > fMax))
			{
				fMax = fTemp;
			}
		}
	}

	return fMax;
}

float CFloatMatrix::Min(CFloatMatrix &amMatrix)
{
	float dMin = amMatrix(0, 0);

	int iRows = amMatrix.Rows();
	int iCols = amMatrix.Columns();

	float dTemp;

	for (int iRow = 0; iRow < iRows; iRow++)
	{
		for (int iCol = 0; iCol < iCols; iCol++)
		{
			dTemp = amMatrix(iRow, iCol);
			if ((dTemp == dTemp) && (dTemp < dMin))
			{
				dMin = dTemp;
			}
		}
	}

	return dMin;
}

float CFloatMatrix::Sum(CFloatMatrix &amMatrix)
{
	int iRows = amMatrix.Rows();
	int iCols = amMatrix.Columns();
	float dSum = 0;
	for (int iRow = 0; iRow < iRows; iRow++)
	{
		for (int iCol = 0; iCol < iCols; iCol++)
		{
			dSum += amMatrix(iRow, iCol);
		}
	}

	return dSum;
}

float CFloatMatrix::Var(CFloatMatrix &amMatrix)
{
	unsigned int uiLen = 0;
	double dSum = 0;
	int iRows = amMatrix.Rows();
	int iCols = amMatrix.Columns();
	for (int iRow = 0; iRow < iRows; iRow++)
	{
		for (int iCol = 0; iCol < iCols; iCol++)
		{
			double dTemp = amMatrix(iRow, iCol);
			if (dTemp == dTemp)
			{
				dSum += dTemp;
				uiLen++;
			}
		}
	}

	switch (uiLen)
	{
	case 0:
		return 0;
	case 1:
		return 0;
	}

	double dMean = dSum / uiLen;

	dSum = 0;
	for (int iRow = 0; iRow < iRows; iRow++)
	{
		for (int iCol = 0; iCol < iCols; iCol++)
		{
			double dTemp = amMatrix(iRow, iCol);
			if (dTemp == dTemp)
			{
				dTemp -= dMean;
				dSum += (dTemp * dTemp);
			}
		}
	}

	return float(dSum / (uiLen - 1));
}
