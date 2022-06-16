#include "MainFuntionProcess.h"

// 初始化
FuntionProcess::FuntionProcess()
{
	;
}
// 初始化，輸入所有函數
FuntionProcess::FuntionProcess(vector<string> iniInputList)
{
	InputProcess(iniInputList);
}

// 初始化，輸入所有函數，且有輸入計算的範圍
FuntionProcess::FuntionProcess(vector<string> iniInputList, double start, double end)
{
	InputProcess(iniInputList);
	CalculateAllFuntion(start, end);
}



// 處理輸入
void FuntionProcess::InputProcess(vector<string> iniInputList)
{
	// 將輸入放進inputList
	this->inputList.resize(iniInputList.size());
	for (int iniInput = 0; iniInput < iniInputList.size(); iniInput++)
	{
		this->inputList[iniInput].input = iniInputList[iniInput];
		this->inputList[iniInput].errorType = 0;
	}

	// 對輸入的式子進行逐條處理
	for (int ini = 0; ini < iniInputList.size(); ini++)
	{
		// 若輸入式不為空
		if (!(iniInputList[ini].empty() || iniInputList[ini] == "\t"))
		{
			// 消除式子間的空白
			string iniFormula, tmp;
			istringstream ss(iniInputList[ini]);
			while (ss >> tmp)
			{
				iniFormula += tmp;
			}

			// 判斷是 方程式 還是 變數
			if (iniFormula.size() >= 2)
			{
				// 方程式
				if (iniFormula[0] == 'y' && iniFormula[1] == '=')
				{
					// 先把所有funtion分類進funtionList
					iniFormula.erase(0, 2);
					this->inputList[ini].input = iniFormula;
					this->inputList[ini].isFuntion = true;
				}
				else
				{
					this->inputList[ini].isFuntion = false;

					// 測試變數名稱是否合法
					bool errorVariableName = false;
					bool hasEqual = false;
					string variableTmp;
					int test = 0;
					for (; test < iniFormula.size(); test++)
					{
						if (iniFormula[test] == '=')
						{
							hasEqual = true;
							break;
						}
						else if (!((iniFormula[test] >= 'a' && iniFormula[test] <= 'z') || (iniFormula[test] >= 'A' && iniFormula[test] <= 'Z') || (iniFormula[test] >= '0' && iniFormula[test] <= '9')))
						{
							// 變數名稱錯誤
							errorVariableName = true;
							this->inputList[ini].errorType = 13;
							break;
						}

						variableTmp += iniFormula[test];
					}

					// 變數
					if (errorVariableName == false && hasEqual == true)
					{
						// 處理變數
						iniFormula.erase(0, variableTmp.size());

						Calculate tmp(iniFormula);

					}
					// 錯誤
					else if (errorVariableName == true || hasEqual == false)
					{
						this->inputList[ini].errorType = 13;
						continue;
					}
				}
			}
		}
	}
}

// 給定範圍進行計算
void FuntionProcess::CalculateAllFuntion(double start, double end)
{
	for (int round = 0; round < this->inputList.size(); round++)
	{
		if (this->inputList[round].isFuntion)
		{
			Calculate tmp(this->inputList[round].input);

			this->inputList[round].input = tmp.GetCError();

			if (tmp.GetCError() == 0)
			{
				this->drawList.push_back(tmp.GetAnsList(start, end));
			}
		}
	}
}

// 獲取錯誤碼清單
vector<int> FuntionProcess::GetErrorList()
{
	vector<int> errorList;

	// 將錯誤碼提出來
	for (int i = 0; i < this->inputList.size(); i++)
	{
		errorList.push_back(inputList[i].errorType);
	}

	return errorList;
}

// 獲取要繪製的座標清單
vector<vector<Pos>> FuntionProcess::GetDrawList()
{
	return this->drawList;
}