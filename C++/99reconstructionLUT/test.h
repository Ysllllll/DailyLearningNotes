#include <iostream>
#include <vector>

double **NewSpace(int Height, int Width)
{
	double **arr = new double *[Height];
	for (int i = 0; i < Height; i++)
	{
		arr[i] = new double[Width];
	}
	return arr;
}
void DeleteSpace(double **&DeletePointer, int Height, int Width)
{
	for (int i = 0; i < Height; i++)
	{
		delete[] DeletePointer[i];
	}
	delete[] DeletePointer;
	DeletePointer = nullptr;
}

class ReconstructionLUT
{
public:
	//构造、析构
	ReconstructionLUT(int Height = 1024, int Width = 1280);
	ReconstructionLUT(double **CameraM, double **ProjectM, int Height = 1024, int Width = 1280);
	~ReconstructionLUT();

private:
	//输入
	int m_Height;
	int m_Width;
	double **m_CameraM;
	double **m_ProjectM;

public:
	//输入参数设定、获取
	void setCameraM(double **CameraM) { m_CameraM = CameraM; }
	void setProjectM(double **ProjectM) { m_ProjectM = ProjectM; }
	double **getCameraM() { return m_CameraM; }
	double **getProjectM() { return m_ProjectM; }
	void setHeight(int Height) { m_Height = Height; }
	void setWidth(int Width) { m_Width = Width; }
	int getHeight() { return m_Height; }
	int getWidth() { return m_Width; }

private:
	//输出
	double **m_Xpe;
	double **m_JXw;
	double **m_JYw;
	double **m_JZw;
	double m_Xp0;
	double m_Xoc;
	double m_Yoc;
	double m_Zoc;

public:
	//输出参数计算
	void CalculationLUT();
	void CalculationOpticalCenter();
	void CalculationPole();
};
