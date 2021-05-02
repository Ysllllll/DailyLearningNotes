#include "test.h"

int main()
{
	double **CameraM = NewSpace(3, 4);
	double **ProjectM = NewSpace(2, 4);
	CameraM[1][1] = 9;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			std::cout << CameraM[i][j] << " ";
		}
		std::cout << std::endl;
	}

	return 0;
}