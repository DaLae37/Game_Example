//
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr
//
#include "KhuGleWin.h"
#include "KhuGleSignal.h"
#include <iostream>
#include <random>
#include <chrono>

#pragma warning(disable:4996)

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#include <fstream>
#include <string>
#include <algorithm>

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

class CKhuGleRocLayer : public CKhuGleLayer
{
public:
	std::vector<std::pair<int, double>> m_Data;
	std::vector<std::pair<double, double>> m_Positive;

	std::vector<std::string> m_Head;
	std::vector<std::vector<double>> m_ReadData;
	CKhuGleRocLayer(int nW, int nH, KgColor24 bgColor, CKgPoint ptPos = CKgPoint(0, 0))
		: CKhuGleLayer(nW, nH, bgColor, ptPos)
	{
	}
	void MakeData();
	void ComputePositives();
	void DrawBackgroundImage();
};

void ReadCsv(std::string FileName, std::vector<std::string>& Head, std::vector<std::vector<double>>& Data) {
	Head.clear();
	Data.clear();

	std::ifstream ifs;

	ifs.open(FileName);
	if (!ifs.is_open()) return;

	std::string LineString = "";
	std::string Delimeter = ",";
	bool bHead = true;
	while (getline(ifs, LineString))
	{
		std::vector<double> RowData;

		unsigned int nPos = 0, nFindPos;
		do {
			nFindPos = LineString.find(Delimeter, nPos);
			if (nFindPos == std::string::npos) nFindPos = LineString.length();

			if (bHead)
				Head.push_back(LineString.substr(nPos, nFindPos - nPos));
			else
				RowData.push_back(std::stod(LineString.substr(nPos, nFindPos - nPos)));

			nPos = nFindPos + 1;
		} while (nFindPos < LineString.length());

		if (bHead) {
			bHead = false;
		}
		else
			Data.push_back(RowData);
	}

	ifs.close();
}

bool DistanceSort(std::pair<double, int>a, std::pair<double, int>b) {
	return a.first < b.first;
}

void CKhuGleRocLayer::MakeData()
{
	unsigned int seed = (unsigned int)std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	std::poisson_distribution<int> poisson_dist(85);

	m_Data.clear();

	std::vector<bool> Train(m_ReadData.size());
	int nTrain = 0, nTest = 0;
	for(int i=0; i<m_ReadData.size(); i++){
		if (rand() % 5 < 4) {
			Train[i] = true;
			nTrain++;
		}
		else {
			Train[i] = false;
			nTest++;
		}
	}

	std::cout << "\n------------------------------" << std::endl;
	std::cout << "Total data : " << m_ReadData.size() << std::endl;
	std::cout << "Train data : " << nTrain << std::endl;
	std::cout << "Test data : " << nTest << std::endl;

	int k = rand() % 7 * 2 + 3;
	float thresh = k / (float)2;
	std::cout << "Test : tau > k / 2 (k = " << k << ")" << std::endl;
	double nTP = 0, nFP = 0, nTN = 0, positive = 0, negative = 0;
	for (int i = 0; i < m_ReadData.size(); i++) {
		if (Train[i]) {
			continue;
		}

		std::vector<std::pair<double, int>> NN;
		for (int j = 0; j < m_ReadData.size(); j++) {
			double dist = 0;
			if (!Train[j]) {
				continue;
			}
			for (int k = 0; k < m_ReadData[j].size() - 1; k++) {
				dist += (m_ReadData[i][k] - m_ReadData[j][k]) * (m_ReadData[i][k] - m_ReadData[j][k]);
			}
			dist = sqrt(dist);
			NN.push_back({ dist, (int)m_ReadData[j][m_ReadData[j].size() - 1]});
		}
		std::sort(NN.begin(), NN.end(), DistanceSort);

		int decision = 0;

		for (int j = 0; j < k; j++) {
			decision += NN[j].second;
		}

		m_Data.push_back({ (int)m_ReadData[i][m_ReadData[i].size() - 1], (double)decision / (double)k });
		if (decision > thresh) {
			decision = 1;
		}
		else {
			decision = 0;
		}

		int real = (int)m_ReadData[i][m_ReadData[i].size() - 1];
		if (decision == 1) {
			if (real == 1) {
				nTP++;
			}
			else {
				nFP++;
			}
			positive++;
		}
		else {
			if (real == 0) {
				nTN++;
			}
			negative++;
		}
	}

	std::cout << "True Positive Rate : " << nTP / positive * 100 << std::endl;
	std::cout << "False Positive Rate : " << nFP / negative * 100<< std::endl;
	std::cout << "Accuracy : " << (nTP + nTN) / (positive + negative) * 100 << std::endl;
}

void CKhuGleRocLayer::ComputePositives()
{
	m_Positive.clear();

	for (int nThreshold = 0; nThreshold <= 100; nThreshold += 1)
	{
		double TP = 0, FP = 0;
		int nPositiveCnt = 0;
		for (auto& Data : m_Data)
		{
			if (Data.second >= nThreshold / 100.)
			{
				if (Data.first == 1)
					TP++;
				else
					FP++;
			}

			if (Data.first == 1)
				nPositiveCnt++;
		}

		TP /= nPositiveCnt;
		FP /= (m_Data.size() - nPositiveCnt);

		m_Positive.push_back({ TP, FP });
	}	
}

void CKhuGleRocLayer::DrawBackgroundImage()
{
	for(int y = 0 ; y < m_nH ; y++)
		for(int x = 0 ; x < m_nW ; x++)
		{
			m_ImageBgR[y][x] = KgGetRed(m_bgColor);
			m_ImageBgG[y][x] = KgGetGreen(m_bgColor);
			m_ImageBgB[y][x] = KgGetBlue(m_bgColor);
		}

	int xx0, yy0, xx1, yy1;
	bool bFirst = true;
	for(auto &Positive : m_Positive)
	{
		xx1 = (int)(Positive.second * (m_nW-1));
		yy1 = m_nH-(int)(Positive.first * (m_nH-1))-1;

		if(!bFirst)
			CKhuGleSprite::DrawLine(m_ImageBgR, m_ImageBgG, m_ImageBgB, m_nW, m_nH, 
				xx0, yy0, xx1, yy1, KG_COLOR_24_RGB(0, 255, 0));

		bFirst = false;
		
		xx0 = xx1;
		yy0 = yy1;
	}
}

class CPerformance : public CKhuGleWin
{
public:
	CKhuGleRocLayer *m_pRocLayer;
		
	CPerformance(int nW, int nH);
	void Update();
};

CPerformance::CPerformance(int nW, int nH) : CKhuGleWin(nW, nH) 
{
	m_pScene = new CKhuGleScene(520, 530, KG_COLOR_24_RGB(100, 100, 150));

	m_pRocLayer = new CKhuGleRocLayer(480, 480, KG_COLOR_24_RGB(150, 150, 200), CKgPoint(20, 30));
	m_pScene->AddChild(m_pRocLayer);

	std::string CsvPath;
	CsvPath = std::string("..\\Run\\diabetes.csv");
	ReadCsv(CsvPath, m_pRocLayer->m_Head, m_pRocLayer->m_ReadData);

	for (auto& column : m_pRocLayer->m_Head) {
		std::cout << column << ", ";
	}
	std::cout << std::endl;

	std::vector<double> mean(m_pRocLayer->m_Head.size() - 1);
	std::vector<double> sd(m_pRocLayer->m_Head.size() - 1);
	for (auto& read : m_pRocLayer->m_ReadData) {
		for (int k = 0; k < mean.size(); ++k) {
			mean[k] += read[k];
			sd[k] += read[k] * read[k];
		}
	}

	for (int k = 0; k < mean.size(); ++k) {
		mean[k] /= m_pRocLayer->m_ReadData.size();

		sd[k] = sd[k] / m_pRocLayer->m_ReadData.size() - mean[k] * mean[k];
		sd[k] = sqrt(sd[k]);
	}

	int n = 0;
	for (auto& read : m_pRocLayer->m_ReadData) {
		for (auto& column : read) {
			std::cout << column << ", ";
		}
		std::cout << std::endl;

		if (n++ > 10) {
			break;
		}
	}
	for (auto& read : m_pRocLayer->m_ReadData) {
		for (int k = 0; k < mean.size(); ++k) {
			read[k] = (read[k] - mean[k]) / sd[k];
		}
	}

	std::cout << "\nZ-socre normalization" << std::endl;
	n = 0;
	for (auto& read : m_pRocLayer->m_ReadData) {
		for (auto& column : read) {
			std::cout << column << ", ";
		}
		std::cout << std::endl;

		if (n++ > 10) {
			break;
		}
	}

	m_pRocLayer->MakeData();
	m_pRocLayer->ComputePositives();
	m_pRocLayer->DrawBackgroundImage();
}

void CPerformance::Update()
{
	if(m_bKeyPressed['N'])
	{
		m_pRocLayer->MakeData();
		m_pRocLayer->ComputePositives();
		m_pRocLayer->DrawBackgroundImage();

		m_bKeyPressed['N'] = false;
	}

	unsigned int seed = (unsigned int)std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);

	m_pScene->Render();
	DrawSceneTextPos("K-NN Classification", CKgPoint(0, 0));

	CKhuGleWin::Update();
}

int main()
{
	CPerformance *pPerformance = new CPerformance(520, 530);

	KhuGleWinInit(pPerformance);

	return 0;
}