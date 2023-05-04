//
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr
//
#include "KhuGleWin.h"
#include "KhuGleSignal.h"
#include <iostream>

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

#include <conio.h> // 파일 입력
#include <iomanip> // 히스토그램
#include <string> // 히스토그램
#include <tgmath.h> // log2

class CKhuGleImageLayer : public CKhuGleLayer {
public:
	CKhuGleSignal m_Image, m_Image_YCbCr, m_Image_DCT, m_Image_Q, m_ImageOut;

	CKhuGleImageLayer(int nW, int nH, KgColor24 bgColor, CKgPoint ptPos = CKgPoint(0, 0))
		: CKhuGleLayer(nW, nH, bgColor, ptPos) {}
	void DrawBackgroundImage();
	void DrawImage();
};

void CKhuGleImageLayer::DrawBackgroundImage()
{
	for(int y = 0 ; y < m_nH ; y++)
		for(int x = 0 ; x < m_nW ; x++)
		{
			m_ImageBgR[y][x] = KgGetRed(m_bgColor);
			m_ImageBgG[y][x] = KgGetGreen(m_bgColor);
			m_ImageBgB[y][x] = KgGetBlue(m_bgColor);
		}
}

void CKhuGleImageLayer::DrawImage() {
	int OffsetX = 130, OffsetY = 10;
	if (m_Image.m_Red && m_Image.m_Green && m_Image.m_Blue)
	{
		for (int y = 0; y < m_Image.m_nH && y < m_nH; ++y)
			for (int x = 0; x < m_Image.m_nW && x < m_nW; ++x)
			{
				m_ImageBgR[y + OffsetY][x + OffsetX] = m_Image.m_Red[y][x];
				m_ImageBgG[y + OffsetY][x + OffsetX] = m_Image.m_Green[y][x];
				m_ImageBgB[y + OffsetY][x + OffsetX] = m_Image.m_Blue[y][x];
			}
	}

	if (m_ImageOut.m_Red && m_ImageOut.m_Green && m_ImageOut.m_Blue)
	{
		OffsetX = 540;
		OffsetY = 10;
		for (int y = 0; y < m_ImageOut.m_nH && y + OffsetY < m_nH; ++y)
			for (int x = 0; x < m_ImageOut.m_nW && x + OffsetX < m_nW; ++x)
			{
				m_ImageBgR[y + OffsetY][x + OffsetX] = m_ImageOut.m_Red[y][x];
				m_ImageBgG[y + OffsetY][x + OffsetX] = m_ImageOut.m_Green[y][x];
				m_ImageBgB[y + OffsetY][x + OffsetX] = m_ImageOut.m_Blue[y][x];
			}
	}

	if (m_Image_YCbCr.m_Red && m_Image_YCbCr.m_Green && m_Image_YCbCr.m_Blue)
	{
		OffsetX = 10;
		OffsetY = 310;
		for (int y = 0; y < m_ImageOut.m_nH && y + OffsetY < m_nH; ++y)
			for (int x = 0; x < m_ImageOut.m_nW && x + OffsetX < m_nW; ++x)
			{
				m_ImageBgR[y + OffsetY][x + OffsetX] = m_Image_YCbCr.m_Red[y][x];
				m_ImageBgG[y + OffsetY][x + OffsetX] = m_Image_YCbCr.m_Green[y][x];
				m_ImageBgB[y + OffsetY][x + OffsetX] = m_Image_YCbCr.m_Blue[y][x];
			}
	}

	if (m_Image_DCT.m_Red && m_Image_DCT.m_Green && m_Image_DCT.m_Blue)
	{
		OffsetX = 310;
		OffsetY = 310;
		for (int y = 0; y < m_ImageOut.m_nH && y + OffsetY < m_nH; ++y)
			for (int x = 0; x < m_ImageOut.m_nW && x + OffsetX < m_nW; ++x)
			{
				m_ImageBgR[y + OffsetY][x + OffsetX] = m_Image_DCT.m_Red[y][x];
				m_ImageBgG[y + OffsetY][x + OffsetX] = m_Image_DCT.m_Green[y][x];
				m_ImageBgB[y + OffsetY][x + OffsetX] = m_Image_DCT.m_Blue[y][x];
			}
	}

	if (m_Image_Q.m_Red && m_Image_Q.m_Green && m_Image_Q.m_Blue)
	{
		OffsetX = 620;
		OffsetY = 310;
		for (int y = 0; y < m_ImageOut.m_nH && y + OffsetY < m_nH; ++y)
			for (int x = 0; x < m_ImageOut.m_nW && x + OffsetX < m_nW; ++x)
			{
				m_ImageBgR[y + OffsetY][x + OffsetX] = m_Image_Q.m_Red[y][x];
				m_ImageBgG[y + OffsetY][x + OffsetX] = m_Image_Q.m_Green[y][x];
				m_ImageBgB[y + OffsetY][x + OffsetX] = m_Image_Q.m_Blue[y][x];
			}
	}
}

class CImageProcessing : public CKhuGleWin {
private :
	bool isLoaded;
	double QuantizationTable_Y[8][8] = {
		{ 16, 11, 10, 16, 24, 40, 51, 61 },
		{ 12, 12, 14, 19, 26, 58, 60, 55 },
		{ 14, 13, 16, 24, 40, 57, 69, 56 },
		{ 14, 17, 22, 29, 51, 87, 80, 62 },
		{ 18, 22, 37, 56, 68, 109, 103, 77 },
		{ 24, 35, 55, 64, 81, 104, 113, 92 },
		{ 49, 64, 78, 87, 103, 121, 120, 101 },
		{ 72, 92, 95, 98, 112, 100, 103, 99 }
	};
	double QuantizationTable_CbCr[8][8] = {
		{ 17, 18, 24, 47, 99, 99, 99, 99 },
		{ 18, 21, 26, 66, 99, 99, 99, 99 },
		{ 24, 26, 56, 99, 99, 99, 99, 99 },
		{ 47, 66, 99, 99, 99, 99, 99, 99 },
		{ 99, 99, 99, 99, 99, 99, 99, 99 },
		{ 99, 99, 99, 99, 99, 99, 99, 99 },
		{ 99, 99, 99, 99, 99, 99, 99, 99 },
		{ 99, 99, 99, 99, 99, 99, 99, 99 }
	};
	int numberOfZero_Y, Max_Y, Min_Y;
	int numberOfZero_Cb, Max_Cb, Min_Cb;
	int numberOfZero_Cr, Max_Cr, Min_Cr;
public:
	CKhuGleImageLayer *m_pImageLayer;

	CImageProcessing(int nW, int nH);
	void Update();
	void LoadFile();

	void RGBtoYCbCr(double** R, double** G, double** B, double** Y, double** Cb, double** Cr);
	void YCbCrtoRGB(double** Y, double** Cb, double** Cr, double** R, double** G, double** B);

	void DCT(double** Y, double** Cb, double** Cr, double** DCT_Y, double** DCT_Cb, double** DCT_Cr);
	void IDCT(double** DCT_Y, double** DCT_Cb, double** DCT_Cr, double** Y, double** Cb, double** Cr);

	void Quantization(double** DCT_Y, double** DCT_Cb, double** DCT_Cr, double** Q_Y, double** Q_Cb, double** Q_Cr);
	void IQuantization(double** Q_Y, double** Q_Cb, double** Q_Cr, double** DCT_Y, double** DCT_Cb, double** DCT_Cr);

	void PrintQuantization(double** Q_Y, double** Q_Cb, double** Q_Cr);
	void PSNR();
};

CImageProcessing::CImageProcessing(int nW, int nH) : CKhuGleWin(nW, nH) {
	isLoaded = false;
	numberOfZero_Y = numberOfZero_Cb = numberOfZero_Cr = 0;
	Max_Y = Max_Cb = Max_Cr = -9999;
	Min_Y = Min_Cb = Min_Cr = 9999;
	m_pScene = new CKhuGleScene(960, 640, KG_COLOR_24_RGB(100, 100, 150));

	m_pImageLayer = new CKhuGleImageLayer(920, 600, KG_COLOR_24_RGB(150, 150, 200), CKgPoint(20, 30));
	m_pImageLayer->DrawBackgroundImage();
	m_pScene->AddChild(m_pImageLayer);
}

void CImageProcessing::LoadFile() {
	char ExePath[MAX_PATH], ImagePath[MAX_PATH];
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

	std::string imageName;
	std::cin >> imageName; //Interrupt Zone
	sprintf(ImagePath, "%s\\%s", ExePath, imageName.c_str());

	bool result = true;
	m_pImageLayer->m_Image.ReadBmp(ImagePath, &result);
	m_pImageLayer->m_Image_YCbCr.ReadBmp(ImagePath, &result);
	m_pImageLayer->m_Image_DCT.ReadBmp(ImagePath, &result);
	m_pImageLayer->m_Image_Q.ReadBmp(ImagePath, &result);
	m_pImageLayer->m_ImageOut.ReadBmp(ImagePath, &result);

	if (!result) {
		std::cout << "파일이 없습니다." << std::endl;
	}
	else {
		isLoaded = true;
		std::cout << "로딩 완료, 데이터 크기 : " << m_pImageLayer->m_Image.m_nH * m_pImageLayer->m_Image.m_nH << std::endl;
	}
}

void CImageProcessing::RGBtoYCbCr(double** R, double** G, double** B, double** Y, double** Cb, double** Cr) {
	for (int y = 0; y < m_pImageLayer->m_ImageOut.m_nH; ++y) {
		for (int x = 0; x < m_pImageLayer->m_ImageOut.m_nW; ++x)
		{
			Y[y][x] = R[y][x] * 0.29900 + G[y][x] * 0.58700 + B[y][x] * 0.11400;

			if (y % 2 == 0 && x % 2 == 0) {
				double sumR = (R[y][x] + R[y + 1][x] + R[y][x + 1] + R[y + 1][x + 1]) / 4;
				double sumG = (G[y][x] + G[y + 1][x] + G[y][x + 1] + G[y + 1][x + 1]) / 4;
				double sumB = (B[y][x] + B[y + 1][x] + B[y][x + 1] + B[y + 1][x + 1]) / 4;

				Cb[y / 2][x / 2] = sumR * -0.16874 + sumG * -0.33126 + sumB * 0.50000;
				Cr[y / 2][x / 2] = sumR * 0.50000 + sumG * -0.41869 + sumB * -0.08131;
			}

			m_pImageLayer->m_Image_YCbCr.m_Red[y][x] = Cr[y / 2][x / 2];
			m_pImageLayer->m_Image_YCbCr.m_Green[y][x] = Y[y][x];
			m_pImageLayer->m_Image_YCbCr.m_Blue[y][x] = Cb[y / 2][x / 2];
		}
	}
	std::cout << "RGB to YCbCr 완료, 데이터 크기 : " << (m_pImageLayer->m_Image.m_nW * m_pImageLayer->m_Image.m_nH) / 2 << std::endl;
}

void CImageProcessing::YCbCrtoRGB(double** Y, double** Cb, double** Cr, double** R, double** G, double** B) {
	for (int y = 0; y < m_pImageLayer->m_ImageOut.m_nH; ++y) {
		for (int x = 0; x < m_pImageLayer->m_ImageOut.m_nW; ++x)
		{
			R[y][x] = Y[y][x] + Cr[y / 2][x / 2] * 1.40200;
			if (R[y][x] > 255)
				R[y][x] = 255;
			else if (R[y][x] < 0)
				R[y][x] = 0;

			G[y][x] = Y[y][x] + Cb[y / 2][x / 2] * -0.34414 + Cr[y / 2][x / 2] * -0.71414;
			if (G[y][x] > 255)
				G[y][x] = 255;
			else if (G[y][x] < 0)
				G[y][x] = 0;

			B[y][x] = Y[y][x] + Cb[y / 2][x / 2] * 1.77200;
			if (B[y][x] > 255)
				B[y][x] = 255;
			else if (B[y][x] < 0)
				B[y][x] = 0;
		}
	}
	std::cout << "YCbCr to RGB 완료" << std::endl;
}

void CImageProcessing::DCT(double** Y, double** Cb, double** Cr, double** DCT_Y, double** DCT_Cb, double** DCT_Cr) {
	DCT2D(Y, DCT_Y, m_pImageLayer->m_Image.m_nW, m_pImageLayer->m_Image.m_nH, 8);
	DCT2D(Cb, DCT_Cb, m_pImageLayer->m_Image.m_nW / 2, m_pImageLayer->m_Image.m_nH / 2, 8);
	DCT2D(Cr, DCT_Cr, m_pImageLayer->m_Image.m_nW / 2, m_pImageLayer->m_Image.m_nH / 2, 8);

	for (int y = 0; y < m_pImageLayer->m_ImageOut.m_nH; ++y) {
		for (int x = 0; x < m_pImageLayer->m_ImageOut.m_nW; ++x)
		{
			m_pImageLayer->m_Image_DCT.m_Red[y][x] = DCT_Cr[y / 2][x / 2];
			m_pImageLayer->m_Image_DCT.m_Green[y][x] = DCT_Y[y][x];
			m_pImageLayer->m_Image_DCT.m_Blue[y][x] = DCT_Cb[y / 2][x / 2];
		}
	}

	std::cout << "8 X 8 Block DCT 완료" << std::endl;
}

void CImageProcessing::IDCT(double** DCT_Y, double** DCT_Cb, double** DCT_Cr, double** Y, double** Cb, double** Cr) {
	IDCT2D(DCT_Y, Y, m_pImageLayer->m_Image.m_nW, m_pImageLayer->m_Image.m_nH, 8);
	IDCT2D(DCT_Cb, Cb, m_pImageLayer->m_Image.m_nW / 2, m_pImageLayer->m_Image.m_nH / 2, 8);
	IDCT2D(DCT_Cr, Cr, m_pImageLayer->m_Image.m_nW / 2, m_pImageLayer->m_Image.m_nH / 2, 8);

	for (int y = 0; y < m_pImageLayer->m_ImageOut.m_nH; ++y) {
		for (int x = 0; x < m_pImageLayer->m_ImageOut.m_nW; ++x)
		{
			if (Y[y][x] < 0)
				Y[y][x] = 0;
			else if (Y[y][x] > 255)
				Y[y][x] = 255;

			if (y % 2 == 0 && x % 2 == 0) {
				if (Cb[y / 2][x / 2] < 0)
					Cb[y / 2][x / 2] = 0;
				else if (Cb[y / 2][x / 2] > 255)
					Cb[y / 2][x / 2] = 255;

				if (Cr[y /2][x / 2] < 0)
					Cr[y /2][x / 2] = 0;
				else if (Cr[y / 2][x / 2] > 255)
					Cr[y / 2][x / 2] = 255;
			}

		}
	}

	std::cout << "IDCT 완료" << std::endl;
}

void CImageProcessing::Quantization(double** DCT_Y, double** DCT_Cb, double** DCT_Cr, double** Q_Y, double** Q_Cb, double** Q_Cr) {
	for (int y = 0; y < m_pImageLayer->m_ImageOut.m_nH; ++y) {
		for (int x = 0; x < m_pImageLayer->m_ImageOut.m_nW; ++x)
		{
			Q_Y[y][x] = round(DCT_Y[y][x] / QuantizationTable_Y[y % 8][x % 8]);

			numberOfZero_Y += ((Q_Y[y][x] == 0) ? 1 : 0);
			Max_Y = (Max_Y > Q_Y[y][x]) ? Max_Y : Q_Y[y][x];
			Min_Y = (Min_Y < Q_Y[y][x]) ? Min_Y : Q_Y[y][x];

			if (y % 2 == 0 && x % 2 == 0) {
				Q_Cb[y / 2][x / 2] = round(DCT_Cb[y / 2][x / 2] / QuantizationTable_CbCr[y / 2 % 8][x / 2 % 8]);
				Q_Cr[y / 2][x / 2] = round(DCT_Cr[y / 2][x / 2] / QuantizationTable_CbCr[y / 2 % 8][x / 2 % 8]);

				numberOfZero_Cb += ((Q_Cb[y / 2][x / 2] == 0) ? 1 : 0);
				Max_Cb = (Max_Cb > Q_Cb[y / 2][x / 2]) ? Max_Cb : Q_Cb[y / 2][x / 2];
				Min_Cb = (Min_Cb < Q_Cb[y / 2][x / 2]) ? Min_Cb : Q_Cb[y / 2][x / 2];

				numberOfZero_Cr += ((Q_Cr[y / 2][x / 2] == 0) ? 1 : 0);
				Max_Cr = (Max_Cr > Q_Cr[y / 2][x / 2]) ? Max_Cr : Q_Cr[y / 2][x / 2];
				Min_Cr = (Min_Cr < Q_Cr[y / 2][x / 2]) ? Min_Cr : Q_Cr[y / 2][x / 2];
			}

			m_pImageLayer->m_Image_Q.m_Red[y][x] = Q_Cr[y / 2][x / 2];
			m_pImageLayer->m_Image_Q.m_Green[y][x] = Q_Y[y][x];
			m_pImageLayer->m_Image_Q.m_Blue[y][x] = Q_Cb[y / 2][x / 2];
		}
	}
	std::cout << "Quantization 완료" << std::endl;
}

void CImageProcessing::IQuantization(double** Q_Y, double** Q_Cb, double** Q_Cr, double** DCT_Y, double** DCT_Cb, double** DCT_Cr) {
	for (int y = 0; y < m_pImageLayer->m_ImageOut.m_nH; ++y) {
		for (int x = 0; x < m_pImageLayer->m_ImageOut.m_nW; ++x)
		{
			DCT_Y[y][x] = Q_Y[y][x] * QuantizationTable_Y[y % 8][x % 8];

			if (y % 2 == 0 && x % 2 == 0) {
				DCT_Cb[y / 2][x / 2] = Q_Cb[y / 2][x / 2] * QuantizationTable_CbCr[y / 2 % 8][x / 2 % 8];
				DCT_Cr[y / 2][x / 2] = Q_Cr[y / 2][x / 2] * QuantizationTable_CbCr[y / 2 % 8][x / 2 % 8];
			}
		}
	}

	std::cout << "Inverse Quantization 완료" << std::endl;
}

void CImageProcessing::PrintQuantization(double** Q_Y, double** Q_Cb, double** Q_Cr) {
	double probability;
	double entropyY = 0, entropyCb = 0, entropyCr = 0;

	int* numY = new int[Max_Y - Min_Y + 1];
	int* numCb = new int[Max_Cb - Min_Cb + 1];
	int* numCr = new int[Max_Cr - Min_Cr + 1];
	memset(numY, 0, (Max_Y - Min_Y + 1) * sizeof(int));
	memset(numCb, 0, (Max_Cb - Min_Cb + 1) * sizeof(int));
	memset(numCr, 0, (Max_Cr - Min_Cr + 1) * sizeof(int));

	for (int y = 0; y < m_pImageLayer->m_ImageOut.m_nH; ++y) {
		for (int x = 0; x < m_pImageLayer->m_ImageOut.m_nW; ++x)
		{
			numY[int(Q_Y[y][x]) - Min_Y] += 1;

			if (y % 2 == 0 && x % 2 == 0) {
				numCb[int(Q_Cb[y / 2][x / 2]) - Min_Cb] += 1;
				numCr[int(Q_Cr[y / 2][x / 2]) - Min_Cr] += 1;
			}
		}
	}
	
	std::cout << "채널 별 0의 개수, Y : " << numberOfZero_Y << " Cb : " << numberOfZero_Cb << " Cr : " << numberOfZero_Cr << std::endl;

	std::cout.setf(std::ios::left);

	std::cout << "Y의 히스토그램 " << Min_Y << " ~ " << Max_Y << std::endl;
	for (int i = 0; i < Max_Y - Min_Y + 1; i++) {
		if ((i + 1) % 10 == 0) {
			std::cout << std::endl;
		}
		std::string s = std::to_string(i + Min_Y) + " : " + std::to_string(numY[i]);
		std::cout << std::setw(11) << s;

		if (numY[i] != 0) {
			probability = numY[i] / double(m_pImageLayer->m_Image.m_nH * m_pImageLayer->m_ImageOut.m_nW);
			entropyY += (probability * log2(probability));
		}
	}
	std::cout << std::endl;

	std::cout << "Cb의 히스토그램 " << Min_Cb << " ~ " << Max_Cb << std::endl;
	for (int i = 0; i < Max_Cb - Min_Cb + 1; i++) {
		if ((i + 1) % 10 == 0) {
			std::cout << std::endl;
		}
		std::string s = std::to_string(i + Min_Cb) + " : " + std::to_string(numCb[i]);
		std::cout << std::setw(11) << s;
		
		if (numCb[i] != 0) {
			probability = numCb[i] / double(m_pImageLayer->m_Image.m_nH / 2 * m_pImageLayer->m_ImageOut.m_nW / 2);
			entropyCb += (probability * log2(probability));
		}
	}
	std::cout << std::endl;

	std::cout << "Cr의 히스토그램 " << Min_Cr << " ~ " << Max_Cr << std::endl;
	for (int i = 0; i < Max_Cr - Min_Cr + 1; i++) {
		if ((i + 1) % 10 == 0) {
			std::cout << std::endl;
		}
		std::string s = std::to_string(i + Min_Cr) + " : " + std::to_string(numCr[i]);
		std::cout << std::setw(11) << s;

		if (numCr[i] != 0) {
			probability = numCr[i] / double(m_pImageLayer->m_Image.m_nH / 2 * m_pImageLayer->m_ImageOut.m_nW / 2);
			entropyCr += (probability * log2(probability));
		}
	}

	std::cout << std::endl;
	std::cout << "Y의 Entropy : " << entropyY * -1 << " Cb의 Entropy : " << 
		entropyCb * -1 << " Cr의 Entropy : " << entropyCr * -1<< std::endl;

	delete[] numY;
	delete[] numCb;
	delete[] numCr;
}

void CImageProcessing::PSNR() {
	double Psnr = GetPsnr(m_pImageLayer->m_Image.m_Red, m_pImageLayer->m_Image.m_Green, m_pImageLayer->m_Image.m_Blue,
		m_pImageLayer->m_ImageOut.m_Red, m_pImageLayer->m_ImageOut.m_Green, m_pImageLayer->m_ImageOut.m_Blue,
		m_pImageLayer->m_Image.m_nW, m_pImageLayer->m_Image.m_nH);

	std::cout << "PSNR : " << Psnr << std::endl;
}

void CImageProcessing::Update()
{
	m_pScene->Render();
	m_pImageLayer->DrawBackgroundImage();
	DrawSceneTextPos("Image Processing", CKgPoint(0, 0));

	if (!isLoaded) {
		DrawSceneTextPos("Input Your Image File Name at Console", CKgPoint(300, 300));
		if (kbhit()) {
			LoadFile();
		}
	}
	else {
		if (m_bKeyPressed['R']) {
			isLoaded = false;
			std::cout << std::endl;
		}
		if (m_bKeyPressed['C']) {
			//이미지 테이블 생성
			double** R = dmatrix(m_pImageLayer->m_Image.m_nH, m_pImageLayer->m_Image.m_nW);
			double** G = dmatrix(m_pImageLayer->m_Image.m_nH, m_pImageLayer->m_Image.m_nW);
			double** B = dmatrix(m_pImageLayer->m_Image.m_nH, m_pImageLayer->m_Image.m_nW);
			for (int y = 0; y < m_pImageLayer->m_Image.m_nH; ++y) {
				for (int x = 0; x < m_pImageLayer->m_Image.m_nW; ++x)
				{
					R[y][x] = m_pImageLayer->m_Image.m_Red[y][x];
					G[y][x] = m_pImageLayer->m_Image.m_Green[y][x];
					B[y][x] = m_pImageLayer->m_Image.m_Blue[y][x];
				}
			}

			//프로세싱 테이블 생성
			double** Y = dmatrix(m_pImageLayer->m_Image.m_nH, m_pImageLayer->m_Image.m_nW);
			double** Cb = dmatrix(m_pImageLayer->m_Image.m_nH / 2, m_pImageLayer->m_Image.m_nW / 2);
			double** Cr = dmatrix(m_pImageLayer->m_Image.m_nH / 2, m_pImageLayer->m_Image.m_nW / 2);

			double** DCT_Y = dmatrix(m_pImageLayer->m_Image.m_nH, m_pImageLayer->m_Image.m_nW);
			double** DCT_Cb = dmatrix(m_pImageLayer->m_Image.m_nH / 2, m_pImageLayer->m_Image.m_nW / 2);
			double** DCT_Cr = dmatrix(m_pImageLayer->m_Image.m_nH / 2, m_pImageLayer->m_Image.m_nW / 2);

			double** Q_Y = dmatrix(m_pImageLayer->m_Image.m_nH, m_pImageLayer->m_Image.m_nW);
			double** Q_Cb = dmatrix(m_pImageLayer->m_Image.m_nH / 2, m_pImageLayer->m_Image.m_nW / 2);
			double** Q_Cr = dmatrix(m_pImageLayer->m_Image.m_nH / 2, m_pImageLayer->m_Image.m_nW / 2);

			//이미지 처리
			RGBtoYCbCr(R, G, B, Y, Cb, Cr);
			DCT(Y, Cb, Cr, DCT_Y, DCT_Cb, DCT_Cr);
			Quantization(DCT_Y, DCT_Cb, DCT_Cr, Q_Y, Q_Cb, Q_Cr);
			PrintQuantization(Q_Y, Q_Cb, Q_Cr);

			//이미지복원
			IQuantization(Q_Y, Q_Cb, Q_Cr, DCT_Y, DCT_Cb, DCT_Cr);
			IDCT(DCT_Y, DCT_Cb, DCT_Cr, Y, Cb, Cr);
			YCbCrtoRGB(Y, Cb, Cr, R, G, B);

			//결과 저장
			for (int y = 0; y < m_pImageLayer->m_ImageOut.m_nH; ++y)
				for (int x = 0; x < m_pImageLayer->m_ImageOut.m_nW; ++x)
				{
					m_pImageLayer->m_ImageOut.m_Red[y][x] = R[y][x];
					m_pImageLayer->m_ImageOut.m_Green[y][x] = G[y][x];
					m_pImageLayer->m_ImageOut.m_Blue[y][x] = B[y][x];
				}

			//평가
			PSNR();

			//테이블 해제
			free_dmatrix(R, m_pImageLayer->m_Image.m_nH, m_pImageLayer->m_Image.m_nW);
			free_dmatrix(G, m_pImageLayer->m_Image.m_nH, m_pImageLayer->m_Image.m_nW);
			free_dmatrix(B, m_pImageLayer->m_Image.m_nH, m_pImageLayer->m_Image.m_nW);

			free_dmatrix(Y, m_pImageLayer->m_Image.m_nH, m_pImageLayer->m_Image.m_nW);
			free_dmatrix(Cb, m_pImageLayer->m_Image.m_nH / 2, m_pImageLayer->m_Image.m_nW / 2);
			free_dmatrix(Cr, m_pImageLayer->m_Image.m_nH / 2, m_pImageLayer->m_Image.m_nW / 2);

			free_dmatrix(DCT_Y, m_pImageLayer->m_Image.m_nH, m_pImageLayer->m_Image.m_nW);
			free_dmatrix(DCT_Cb, m_pImageLayer->m_Image.m_nH / 2, m_pImageLayer->m_Image.m_nW / 2);
			free_dmatrix(DCT_Cr, m_pImageLayer->m_Image.m_nH / 2, m_pImageLayer->m_Image.m_nW / 2);

			free_dmatrix(Q_Y, m_pImageLayer->m_Image.m_nH, m_pImageLayer->m_Image.m_nW);
			free_dmatrix(Q_Cb, m_pImageLayer->m_Image.m_nH / 2, m_pImageLayer->m_Image.m_nW / 2);
			free_dmatrix(Q_Cr, m_pImageLayer->m_Image.m_nH / 2, m_pImageLayer->m_Image.m_nW / 2);

			m_bKeyPressed['C'] = false;
		}
		m_pImageLayer->DrawImage();
		DrawSceneTextPos("Press C to Start, Pree R to reload", CKgPoint(250, 0));
		DrawSceneTextPos("Origin Image", CKgPoint(220, 300));
		DrawSceneTextPos("After Image", CKgPoint(630, 300));
		DrawSceneTextPos("YCbCr", CKgPoint(120, 600));
		DrawSceneTextPos("DCT", CKgPoint(420, 600));
		DrawSceneTextPos("Quantization", CKgPoint(700, 600));
	}
	CKhuGleWin::Update();
}

int main()
{
	CImageProcessing *pImageProcessing = new CImageProcessing(960, 640);
	KhuGleWinInit(pImageProcessing);

	return 0;
}