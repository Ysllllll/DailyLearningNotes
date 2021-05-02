#ifndef FLOATMATRIX_H
#define FLOATMATRIX_H

#define PI 3.141592653F

#include <iostream>

using namespace std;

class CFloatMatrix
{
public:
	int Rows();
	int Columns();
	int Length();
	float Get(int aiRow, int aiColumn);
	float Get(int aiCount);
	void Set(int aiRow, int aiColumn, float afValue);
	void SetSub(int aiRowStart, int aiRowEnd, int aiColStart, int aiColEnd, CFloatMatrix amSubMat);
	CFloatMatrix GetSub(int aiRowStart, int aiRowEnd, int aiColStart, int aiColEnd);

private:
	int m_iRows;
	int m_iColumns;
	float **m_ppfData;

private:
	void Initialize(int aiRows, int aiColumns, float adInitValue, bool abInit);
	void Initialize(CFloatMatrix &amMatrix);
	void Release(void);

	static float Determinant3(CFloatMatrix amA);

public:
	CFloatMatrix(void);
	CFloatMatrix(CFloatMatrix &amMatrix);
	CFloatMatrix(string astrFileName);
	CFloatMatrix(int aiDimension);
	CFloatMatrix(int aiRows, int aiColumns);
	CFloatMatrix(int aiRows, int aiColumns, float adInitValue);
	~CFloatMatrix(void);

	void Assign(float adValue);
	void Resize(int aiRows, int aiCols);
	void Resize(int aiRows, int aiCols, CFloatMatrix amMatrix);
	void Resize(int aiRows, int aiCols, float adInitValue);

	void SaveAsBinary(string astrFileName);
	void SaveAsBinary(CFloatMatrix amData, string astrFileName);

	static int ParseSubscript(string astrSubscript, int *apiFrom, int *apiTo);
	static int FindChar(string astrSource, char achTarget);
	static int GetSubscript(string astrInt);

	float *operator[](int aiCount);
	float &operator()(int aiRow, int aiCol);
	float &operator()(int aiCount);

	CFloatMatrix operator*(CFloatMatrix amMatrix);
	CFloatMatrix operator*(double adScalar);
	friend CFloatMatrix operator*(double adScalor, CFloatMatrix amMatrix);
	CFloatMatrix &operator=(CFloatMatrix &amMatrix);

	CFloatMatrix operator+(CFloatMatrix amMatrix);
	CFloatMatrix operator+(double adScalor);
	friend CFloatMatrix operator+(double adScalor, CFloatMatrix amMatrix);

	CFloatMatrix operator-(CFloatMatrix amMatrix);
	CFloatMatrix operator-(double adScalor);
	friend CFloatMatrix operator-(double adScalor, CFloatMatrix amMatrix);

	CFloatMatrix operator/(double adScalar);

	CFloatMatrix Transpose(void);
	CFloatMatrix Copy(void);
	void CopyFrom(CFloatMatrix &amMatrix);
	static CFloatMatrix Transpose(CFloatMatrix amMatrix);
	static CFloatMatrix LoadData(string astrFileName);
	static CFloatMatrix Identity(int aiDim);
	static float VectorNorm(CFloatMatrix amVector, int aiNorm);

	static CFloatMatrix ElementAcos(CFloatMatrix amA);
	static CFloatMatrix ElementAtan2(CFloatMatrix amA, CFloatMatrix amB);
	static CFloatMatrix ElementCos(CFloatMatrix amA);
	static CFloatMatrix ElementSin(CFloatMatrix amA);
	static CFloatMatrix ElementPower(CFloatMatrix amMatix, float adPower);
	static CFloatMatrix Inverse(CFloatMatrix &amMatrix);
	static CFloatMatrix Inverse3(CFloatMatrix amA);

	static CFloatMatrix RemoveNaNWithIdx(CFloatMatrix amA, CFloatMatrix amIdx, int aiCount);

	static float Max(CFloatMatrix &amMatrix);
	static float Min(CFloatMatrix &amMatrix);
	static float Sum(CFloatMatrix &amMatrix);
	static float Var(CFloatMatrix &amMatrix);
};

#endif // FLOATMATRIX_H
