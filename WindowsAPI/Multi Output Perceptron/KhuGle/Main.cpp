//
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr
//
#include "KhuGleWin.h"
#include "KhuGleSignal.h"
#include <iostream>

#include "KhuDaNetLayer.h"
#include "KhuDaNet.h"

#pragma warning(disable:4996)

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

class CKhuGleGraphLayer : public CKhuGleLayer
{
public:
	int m_nCurrentCnt;
	std::vector<std::vector<double>> m_Data;
	std::vector<double> m_MaxData;
	int m_nDataTotal;

	CKhuGleGraphLayer(int nW, int nH, KgColor24 bgColor, int nDataTotal, CKgPoint ptPos = CKgPoint(0, 0));
	void SetMaxData(int nIndex, double Value);
	void DrawBackgroundImage();
};

CKhuGleGraphLayer::CKhuGleGraphLayer(int nW, int nH, KgColor24 bgColor, int nDataTotal, CKgPoint ptPos)
	: m_MaxData(nDataTotal), m_Data(nDataTotal), CKhuGleLayer(nW, nH, bgColor, ptPos)
{
	m_bgColor = bgColor;
	m_nCurrentCnt = 0;
	m_nDataTotal = nDataTotal;
}

void CKhuGleGraphLayer::SetMaxData(int nIndex, double Value)
{
	m_MaxData[nIndex] = Value;
}

void CKhuGleGraphLayer::DrawBackgroundImage()
{
	for (int y = 0; y < m_nH; y++)
		for (int x = 0; x < m_nW; x++)
		{
			m_ImageBgR[y][x] = KgGetRed(m_bgColor);
			m_ImageBgG[y][x] = KgGetGreen(m_bgColor);
			m_ImageBgB[y][x] = KgGetBlue(m_bgColor);
		}

	int xx0, yy0, xx1, yy1;
	for (int k = 0; k < m_nDataTotal; ++k)
	{
		KgColor24 Color = KG_COLOR_24_RGB(k % 2 * 255, k / 2 % 2 * 255, k / 4 % 2 * 255);
		for (int i = 0; i < m_nCurrentCnt; ++i) {
			xx1 = i * m_nW / m_nCurrentCnt;
			yy1 = (int)(m_nH - m_Data[k][i] * m_nH / m_MaxData[k] - 1);
			if (yy1 < 0) yy1 = 0;
			if (yy1 >= m_nH) yy1 = m_nH - 1;

			if (i > 0) {
				CKhuGleSprite::DrawLine(m_ImageBgR, m_ImageBgG, m_ImageBgB, m_nW, m_nH,
					xx0, yy0, xx1, yy1, Color);
			}

			xx0 = xx1;
			yy0 = yy1;
		}
	}
}

class CPerceptronTest : public CKhuGleWin
{
public:
	CKhuGleGraphLayer* m_pTrainGraphLayer, * m_pTestGraphLayer;
	bool m_bTrainingRun;

	char m_ExePath[MAX_PATH];
	int m_nBatchCnt, m_nEpochCnt, m_nBatch;
	int m_nMnistTrainTotal, m_nMnistTestTotal;

	double** m_MnistTrainInput, ** m_MnistTestInput;
	int* m_MnistTrainOutput, * m_MnistTestOutput;

	double*** m_W, ** m_B;

	CPerceptronTest(int nW, int nH, char* ExePath);
	~CPerceptronTest();
	void LoadMnistTrain();
	void LoadMnistTest();
	void Update();
};

CPerceptronTest::CPerceptronTest(int nW, int nH, char* ExePath) : CKhuGleWin(nW, nH)
{
	strcpy(m_ExePath, ExePath);

	m_pScene = new CKhuGleScene(640, 480, KG_COLOR_24_RGB(100, 100, 150));

	m_pTrainGraphLayer = new CKhuGleGraphLayer(600, 200, KG_COLOR_24_RGB(150, 150, 200), 6, CKgPoint(20, 30));
	m_pTrainGraphLayer->SetMaxData(0, 100.);
	m_pTrainGraphLayer->SetMaxData(1, 2.5);
	m_pTrainGraphLayer->SetMaxData(2, 100.);
	m_pTrainGraphLayer->SetMaxData(3, 2.5);
	m_pTrainGraphLayer->SetMaxData(4, 100.);
	m_pTrainGraphLayer->SetMaxData(5, 2.5);
	m_pScene->AddChild(m_pTrainGraphLayer);

	m_pTestGraphLayer = new CKhuGleGraphLayer(600, 200, KG_COLOR_24_RGB(150, 150, 200), 3, CKgPoint(20, 260));
	m_pTestGraphLayer->SetMaxData(0, 100.);
	m_pTestGraphLayer->SetMaxData(1, 100.);
	m_pTestGraphLayer->SetMaxData(2, 100.);
	m_pScene->AddChild(m_pTestGraphLayer);

	srand((unsigned)time(0));

	m_W = new double** [3];
	for (int i = 0; i < 3; i++) {
		m_W[i] = new double* [10];
		for (int j = 0; j < 10; j++) {
			m_W[i][j] = new double[28 * 28];
			for (int k = 0; k < 28 * 28; k++) {
				m_W[i][j][k] = (double)rand() / RAND_MAX - 0.5;
			}
		}
	}

	m_B = new double* [3];
	for (int i = 0; i < 3; i++) {
		m_B[i] = new double[10];
		for (int j = 0; j < 10; j++) {
			m_B[i][j] = (double)rand() / RAND_MAX - 0.5;
		}
	}

	m_nBatchCnt = 0;
	m_nEpochCnt = 0;
	m_nBatch = 100;
	m_nMnistTrainTotal = 60000;
	m_nMnistTestTotal = 10000;

	m_MnistTrainInput = m_MnistTestInput = nullptr;
	m_MnistTrainOutput = m_MnistTestOutput = nullptr;

	int i;
	if (!m_MnistTrainInput) {
		m_MnistTrainInput = new double* [m_nMnistTrainTotal];

		for (i = 0; i < m_nMnistTrainTotal; i++)
			m_MnistTrainInput[i] = new double[28 * 28];
	}

	if (!m_MnistTrainOutput)
		m_MnistTrainOutput = new int[m_nMnistTrainTotal];

	if (!m_MnistTestInput) {
		m_MnistTestInput = new double* [m_nMnistTestTotal];

		for (i = 0; i < m_nMnistTestTotal; i++)
			m_MnistTestInput[i] = new double[28 * 28];
	}

	if (!m_MnistTestOutput)
		m_MnistTestOutput = new int[m_nMnistTestTotal];

	LoadMnistTrain();
	LoadMnistTest();

	m_bTrainingRun = false;
}

CPerceptronTest::~CPerceptronTest()
{
	int i;
	if (m_MnistTrainInput) {
		for (i = 0; i < m_nMnistTrainTotal; i++)
			delete[] m_MnistTrainInput[i];

		delete[] m_MnistTrainInput;
	}

	if (m_MnistTrainOutput)
		delete[] m_MnistTrainOutput;

	if (m_MnistTestInput) {
		for (i = 0; i < m_nMnistTestTotal; i++)
			delete[] m_MnistTestInput[i];

		delete[] m_MnistTestInput;
	}

	if (m_MnistTestOutput)
		delete[] m_MnistTestOutput;

	delete[] m_W;
}

void CPerceptronTest::Update()
{
	if (m_bKeyPressed['S'])
	{
		m_bTrainingRun = !m_bTrainingRun;

		m_bKeyPressed['S'] = false;
	}

	if (!m_bTrainingRun)
	{
		m_pScene->Render();
		DrawSceneTextPos("Perceptron Test", CKgPoint(0, 0));

		CKhuGleWin::Update();
		return;
	}

	int nIndex = (m_nBatchCnt * m_nBatch) % m_nMnistTrainTotal;

	if (nIndex + m_nBatch >= m_nMnistTrainTotal)
		nIndex = m_nMnistTrainTotal - m_nBatch;

	int nOutputCnt = 10;

	double** OutputList = new double* [m_nBatch];
	for (int i = 0; i < m_nBatch; ++i)
		OutputList[i] = new double[nOutputCnt];
	for (int i = 0; i < m_nBatch; ++i) {
		for (int j = 0; j < nOutputCnt; ++j) {
			OutputList[i][j] = 0;
			if (m_MnistTrainOutput[nIndex + i] == j) OutputList[i][j] = 1;
		}
	}

	int nTP[3] = { 0,0,0 };
	double Loss[3] = { 0,0,0 };

	double lr = 0.02;
	for (int i = 0; i < 3; i++) {
		for (int k = nIndex; k < nIndex + m_nBatch; ++k) {
			int maxIndex = -1;
			double maxValue = -99999;
			double maxOutput = -1;
			for (int j = 0; j < 10; j++) {
				double y = 0;
				switch (i) {
				case 0:
					for (int x = 0; x < 28 * 28; ++x)
						y += m_MnistTrainInput[k][x] * m_W[i][j][x];
					break;
				case 1:
					for (int x = 0; x < 14 * 14; ++x)
						y += m_MnistTrainInput[k][(x / 14 * 2 + 0) * 28 + x % 14 * 2 + 0]
						* m_W[i][j][x];
					for (int x = 0; x < 14 * 14; ++x)
						y += m_MnistTrainInput[k][(x / 14 * 2 + 1) * 28 + x % 14 * 2 + 1]
						* m_W[i][j][x + 14 * 14];
					for (int x = 0; x < 14 * 14; ++x)
						y += pow(m_MnistTrainInput[k][(x / 14 * 2 + 0) * 28 + x % 14 * 2 + 0], 2)
						* m_W[i][j][x + 14 * 14 * 2];
					for (int x = 0; x < 14 * 14; ++x)
						y += pow(m_MnistTrainInput[k][(x / 14 * 2 + 0) * 28 + x % 14 * 2 + 0], 3)
						* m_W[i][j][x + 14 * 14 * 3];
					break;
				case 2:
					for (int x = 0; x < 14 * 14; ++x)
						y += m_MnistTrainInput[k][(x / 14 * 2 + 0) * 28 + x % 14 * 2 + 0] * m_W[i][j][x];
					for (int x = 0; x < 14 * 14; ++x) {
						double average = 0;
						for (int dx = 0; dx < 2; dx++) {
							for (int dy = 0; dy < 2; dy++) {
								average += m_MnistTrainInput[k][(x / 14 * 2 + dy) * 28 + x % 14 * 2 + dx];
							}
						}
						average /= 4;
						y += average * m_W[i][j][x + 14 * 14];
					}
					for (int x = 0; x < 14 * 14; ++x) {
						double diff = abs(m_MnistTrainInput[k][(x / 14 * 2 + 0) * 28 + x % 14 * 2 + 0]
							- m_MnistTrainInput[k][(x / 14 * 2 + 1) * 28 + x % 14 * 2 + 1]);
						y += diff * m_W[i][j][x + 14 * 14 * 2];
					}
					for (int x = 0; x < 14 * 14; ++x)
						y += pow(m_MnistTrainInput[k][(x / 14 * 2 + 0) * 28 + x % 14 * 2 + 0], 2)
						* m_W[i][j][x + 14 * 14 * 3];
					break;
				}

				y += m_B[i][j];
				y = 1. / (1 + exp(-y));

				double d = OutputList[k - nIndex][j];
				double d2 = (d - y) * (d - y) * 0.5;
				Loss[i] += d2;

				double error = lr * (d - y) * y * (1 - y);

				switch (i) {
				case 0:
					for (int x = 0; x < 28 * 28; ++x)
						m_W[i][j][x] += 
						error * m_MnistTrainInput[k][x];
					break;
				case 1:
					for (int x = 0; x < 14 * 14; ++x)
						m_W[i][j][x] += 
						error * m_MnistTrainInput[k][(x / 14 * 2 + 0) * 28 + x % 14 * 2 + 0];
					for (int x = 0; x < 14 * 14; ++x)
						m_W[i][j][x + 14 * 14] += 
						error * m_MnistTrainInput[k][(x / 14 * 2 + 1) * 28 + x % 14 * 2 + 1];
					for (int x = 0; x < 14 * 14; ++x)
						m_W[i][j][x + 14 * 14 * 2] += 
						error * pow(m_MnistTrainInput[k][(x / 14 * 2 + 0) * 28 + x % 14 * 2 + 0], 2);
					for (int x = 0; x < 14 * 14; ++x)
						m_W[i][j][x + 14 * 14 * 3] += 
						error * pow(m_MnistTrainInput[k][(x / 14 * 2 + 0) * 28 + x % 14 * 2 + 0], 3);
					break;
				case 2:
					for (int x = 0; x < 14 * 14; ++x)
						m_W[i][j][x] += error * m_MnistTrainInput[k][(x / 14 * 2 + 0) * 28 + x % 14 * 2 + 0];
					for (int x = 0; x < 14 * 14; ++x) {
						double average = 0;
						for (int dx = 0; dx < 2; dx++) {
							for (int dy = 0; dy < 2; dy++) {
								average += m_MnistTrainInput[k][(x / 14 * 2 + dy) * 28 + x % 14 * 2 + dx];
							}
						}
						average /= 4;
						m_W[i][j][x + 14 * 14] += error * average;
					}
					for (int x = 0; x < 14 * 14; ++x) {
						double diff = abs(m_MnistTrainInput[k][(x / 14 * 2 + 0) * 28 + x % 14 * 2 + 0] - m_MnistTrainInput[k][(x / 14 * 2 + 1) * 28 + x % 14 * 2 + 1]);
						m_W[i][j][x + 14 * 14 * 2] += error * diff;
					}
					for (int x = 0; x < 14 * 14; ++x)
						m_W[i][j][x + 14 * 14 * 3] += error * pow(m_MnistTrainInput[k][(x / 14 * 2 + 0) * 28 + x % 14 * 2 + 0], 2);
					break;
				}
				m_B[i][j] += error;

				if (y > maxValue) {
					maxValue = y;
					maxIndex = j;
				}
				if (OutputList[k - nIndex][j] == 1)
					maxOutput = j;
			}
			if (maxIndex == maxOutput)
				nTP[i]++;
		}
		Loss[i] /= m_nBatch;
	}

	for (int i = 0; i < m_nBatch; ++i)
		delete[] OutputList[i];
	delete[] OutputList;

	m_nBatchCnt++;
	char Msg[256];
	for (int i = 0; i < 3; i++) {

		sprintf(Msg, "Network#%d, Train accuracy: %6.2lf, %5.3lf(batch index: %5d, total : %6d(%5.1lf), ep(%2d)",
			i + 1, (double)nTP[i] / (double)m_nBatch * 100, Loss[i], m_nBatchCnt, nIndex + m_nBatch,
			(double)(nIndex + m_nBatch) / m_nMnistTrainTotal * 100, m_nEpochCnt + 1);
		std::cout << Msg << std::endl;
	}

	if (nIndex + m_nBatch == m_nMnistTrainTotal)
	{
		m_nEpochCnt++;

		int nTP[3] = { 0,0,0 };

		for (int i = 0; i < 3; i++) {
			for (int k = 0; k < m_nMnistTestTotal; k++) {
				int maxIndex = -1;
				double maxValue = -99999;
				double maxOutput = -1;
				for (int j = 0; j < 10; j++) {
					double y = 0;
					switch (i) {
					case 0:
						for (int x = 0; x < 28 * 28; ++x)
							y += m_MnistTestInput[k][x] * m_W[i][j][x];
						break;
					case 1:
						for (int x = 0; x < 14 * 14; ++x)
							y += m_MnistTestInput[k][(x / 14 * 2 + 0) * 28 + x % 14 * 2 + 0] * m_W[i][j][x];
						for (int x = 0; x < 14 * 14; ++x)
							y += m_MnistTestInput[k][(x / 14 * 2 + 1) * 28 + x % 14 * 2 + 1] * m_W[i][j][x + 14 * 14];
						for (int x = 0; x < 14 * 14; ++x)
							y += pow(m_MnistTestInput[k][(x / 14 * 2 + 0) * 28 + x % 14 * 2 + 0], 2) * m_W[i][j][x + 14 * 14 * 2];
						for (int x = 0; x < 14 * 14; ++x)
							y += pow(m_MnistTestInput[k][(x / 14 * 2 + 0) * 28 + x % 14 * 2 + 0], 3) * m_W[i][j][x + 14 * 14 * 3];
						break;
					case 2:
						for (int x = 0; x < 14 * 14; ++x)
							y += m_MnistTestInput[k][(x / 14 * 2 + 0) * 28 + x % 14 * 2 + 0] * m_W[i][j][x];
						for (int x = 0; x < 14 * 14; ++x) {
							double average = 0;
							for (int dx = 0; dx < 2; dx++) {
								for (int dy = 0; dy < 2; dy++) {
									average += m_MnistTestInput[k][(x / 14 * 2 + dy) * 28 + x % 14 * 2 + dx];
								}
							}
							average /= 4;
							y += average * m_W[i][j][x + 14 * 14];
						}
						for (int x = 0; x < 14 * 14; ++x) {
							double diff = abs(m_MnistTestInput[k][(x / 14 * 2 + 0) * 28 + x % 14 * 2 + 0] - m_MnistTestInput[k][(x / 14 * 2 + 1) * 28 + x % 14 * 2 + 1]);
							y += diff * m_W[i][j][x + 14 * 14 * 2];
						}
						for (int x = 0; x < 14 * 14; ++x)
							y += pow(m_MnistTestInput[k][(x / 14 * 2 + 0)
								* 28 + x % 14 * 2 + 0], 2) * m_W[i][j][x + 14 * 14 * 3];
						break;
					}

					y += m_B[i][j];
					y = 1. / (1 + exp(-y));

					if (y > maxValue) {
						maxValue = y;
						maxIndex = j;
					}
				}
				if (m_MnistTestOutput[k] == maxIndex)
					nTP[i]++;
			}
			sprintf(Msg, "Network#%d, Test accuracy: %7.3lf\n", i + 1, (double)nTP[i] / (double)m_nMnistTestTotal * 100.);
			std::cout << Msg << std::endl;
		}

		m_pTestGraphLayer->m_Data[0].push_back((double)nTP[0] / (double)m_nMnistTestTotal * 100);
		m_pTestGraphLayer->m_Data[1].push_back((double)nTP[1] / (double)m_nMnistTestTotal * 100);
		m_pTestGraphLayer->m_Data[2].push_back((double)nTP[2] / (double)m_nMnistTestTotal * 100);
		m_pTestGraphLayer->m_nCurrentCnt++;
		m_pTestGraphLayer->DrawBackgroundImage();
	}

	m_pTrainGraphLayer->m_Data[0].push_back((double)nTP[0] / (double)m_nBatch * 100);
	m_pTrainGraphLayer->m_Data[1].push_back(Loss[0]);
	m_pTrainGraphLayer->m_Data[2].push_back((double)nTP[1] / (double)m_nBatch * 100);
	m_pTrainGraphLayer->m_Data[3].push_back(Loss[1]);
	m_pTrainGraphLayer->m_Data[4].push_back((double)nTP[2] / (double)m_nBatch * 100);
	m_pTrainGraphLayer->m_Data[5].push_back(Loss[2]);
	m_pTrainGraphLayer->m_nCurrentCnt++;
	m_pTrainGraphLayer->DrawBackgroundImage();

	m_pScene->Render();
	DrawSceneTextPos("Perceptron Test", CKgPoint(0, 0));

	CKhuGleWin::Update();
}

void CPerceptronTest::LoadMnistTrain()
{
	char TrainImagePath[MAX_PATH], TrainLabelPath[MAX_PATH];

	sprintf(TrainImagePath, "%s\\train-images.idx3-ubyte", m_ExePath);
	sprintf(TrainLabelPath, "%s\\train-labels.idx1-ubyte", m_ExePath);

	int i;

	FILE* fp = fopen(TrainImagePath, "rb");
	if (fp)
	{
		unsigned char Buf[28 * 28];
		fread(Buf, 1, 16, fp);

		int nCnt = 0;
		for (i = 0; i < m_nMnistTrainTotal; ++i)
		{
			fread(Buf, 1, 28 * 28, fp);

			int k;
			for (k = 0; k < 28 * 28; k++)
				m_MnistTrainInput[nCnt][k] = (double)Buf[k] / 255.;
			nCnt++;
		}
		fclose(fp);
	}

	fp = fopen(TrainLabelPath, "rb");
	if (fp)
	{
		unsigned char Buf[32];
		fread(Buf, 1, 8, fp);

		int nCnt = 0;
		for (i = 0; i < m_nMnistTrainTotal; ++i)
		{
			fread(Buf, 1, 1, fp);
			m_MnistTrainOutput[nCnt] = Buf[0];
			nCnt++;
		}
		fclose(fp);
	}
}

void CPerceptronTest::LoadMnistTest()
{
	char TestImagePath[MAX_PATH], TestLabelPath[MAX_PATH];

	sprintf(TestImagePath, "%s\\t10k-images.idx3-ubyte", m_ExePath);
	sprintf(TestLabelPath, "%s\\t10k-labels.idx1-ubyte", m_ExePath);

	int i;

	FILE* fp = fopen(TestImagePath, "rb");
	if (fp)
	{
		unsigned char Buf[28 * 28];
		fread(Buf, 1, 16, fp);

		int nCnt = 0;
		for (i = 0; i < m_nMnistTestTotal; ++i)
		{
			fread(Buf, 1, 28 * 28, fp);

			int k;
			for (k = 0; k < 28 * 28; k++)
				m_MnistTestInput[nCnt][k] = (double)Buf[k] / 255.;
			nCnt++;
		}
		fclose(fp);
	}

	fp = fopen(TestLabelPath, "rb");
	if (fp)
	{
		unsigned char Buf[32];
		fread(Buf, 1, 8, fp);

		int nCnt = 0;
		for (i = 0; i < m_nMnistTestTotal; ++i)
		{
			fread(Buf, 1, 1, fp);
			m_MnistTestOutput[nCnt] = Buf[0];
			nCnt++;
		}
		fclose(fp);
	}
}

int main()
{
	char ExePath[MAX_PATH];

	GetModuleFileName(NULL, ExePath, MAX_PATH);

	int i;
	int LastBackSlash = -1;
	int nLen = strlen(ExePath);
	for (i = nLen - 1; i >= 0; i--)
	{
		if (ExePath[i] == '\\') {
			LastBackSlash = i;
			break;
		}
	}

	if (LastBackSlash >= 0)
		ExePath[LastBackSlash] = '\0';

	CPerceptronTest* pPerceptronTest = new CPerceptronTest(640, 480, ExePath);

	KhuGleWinInit(pPerceptronTest);

	return 0;
}