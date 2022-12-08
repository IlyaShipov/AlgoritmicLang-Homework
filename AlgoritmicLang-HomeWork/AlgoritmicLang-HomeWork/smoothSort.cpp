#include "smoothSort.h"

vector<int> LeoNum = { 1, 1, 3, 5, 9, 15, 25, 41, 67, 109, 177, 287, 465, 753, 1219, 1973, 3193, 5167, 8361, 13529, 21891, 35421, 57313, 92735, 150049, 242785, 392835, 635621, 1028457, 1664079, 2692537, 4356617, 7049155, 11405773, 18454929, 29860703, 48315633, 78176337, 126491971, 204668309, 331160281, 535828591, 866988873, 1402817465 };
int curState;

int NextState(int& curState)
{
	int posNewTop = -1; // ������� ������� ������������ ���.
	// ����������
	if ((curState & 7) == 5)
	{ // curState = 0101
		curState += 3; // curState = 1000
		posNewTop = 3;
	}
	else // �������� ����� ��� ������ ��������� ����
	{
		int next = curState;
		int pos = 0;
		while (next && (next & 3) != 3)
		{
			next >>= 1;
			pos++;
		}
		if ((next & 3) == 3) // curState = 01100
		{
			curState += 1 << pos; // curState = 10000
			posNewTop = pos + 2;
		}
		else if (curState & 1) // curState = x001
			curState |= 2; // curState = x011
		else // curState = xx00
			curState |= 1; // curState = xx01
	}
	return posNewTop;
}

void PrevState(int& curState)
{
	if ((curState & 15) == 8)
	{ // curState = 1000
		curState -= 3; // curState = 0101
	}
	else if (curState & 1)
	{
		if ((curState & 3) == 3) // curState = x011
			curState ^= 2; // curState = x001
		else // curState = xx01
			curState ^= 1; // curState = xx00
	}
	else // ���� ������ ��������� ���
	{
		int prev = curState;
		int pos = 0;
		while (prev && !(prev & 1))
		{
			prev >>= 1;
			pos++;
		}
		if (prev) // curState = xx10000
		{
			curState ^= 1 << pos;
			curState |= 1 << (pos - 1);
			curState |= 1 << (pos - 2); // curState = xx01100
		}
		else
			curState = 0;
	}
}

// �������� ����
// mas - ������
// posHeapItemsAmount - ����� LeoNum[posHeapItemsAmount] - ����� ��������� � ������� ����
// indexLastTop - ������ ������� ��������� ����
void shiftDown(vector<int>& mas, int posHeapItemsAmount, int indexLastTop)
{
	// ������� �������� ���� (������� ����)
	int posCurNode = indexLastTop;
	while (posHeapItemsAmount > 1)
	{
		// ������� ������� ����
		int posR = posCurNode - 1;
		// ������� ������ ����
		int posL = posR - LeoNum[posHeapItemsAmount - 2];

		int posMaxChild = posL;
		int posNextTop = posHeapItemsAmount - 1;
		if (mas[posR] > mas[posL])
		{
			posMaxChild = posR;
			posNextTop = posHeapItemsAmount - 2;
		}
		if (mas[posCurNode] < mas[posMaxChild])
		{
			swap(mas[posCurNode], mas[posMaxChild]);
			posHeapItemsAmount = posNextTop;
			posCurNode = posMaxChild;
		}
		else
			break;
	}
}


void make_heap_pool(vector<int>& mas)
{
	for (size_t i = 0; i < mas.size(); i++)
	{
		int posHeapItemsAmount = NextState(curState);
		if (posHeapItemsAmount != -1)
			shiftDown(mas, posHeapItemsAmount, i);
	}
}

// �������: ������ ��������(����� �� ������ ����), ������� ������ ��� ��������� ������� ���
int findPosMaxElem(vector<int>& mas, int curState, int indexLastTop, int& nextPosHeapItemsAmount)
{
	int pos = 0;
	// ���� ������� ������� ���������� ����
	while (!(curState & 1))
	{
		curState >>= 1;
		pos++;
	}

	int posMaxTopElem = indexLastTop;
	nextPosHeapItemsAmount = pos;

	int curTopElem = indexLastTop - LeoNum[pos];

	curState >>= 1;
	pos++;
	while (curState)
	{
		if (curState & 1)
		{
			if (mas[curTopElem] > mas[posMaxTopElem])
			{
				posMaxTopElem = curTopElem;
				nextPosHeapItemsAmount = pos;
			}
			curTopElem -= LeoNum[pos];
		}
		curState >>= 1;
		pos++;
	}

	return posMaxTopElem;
}


vector<int> smoothSort(vector<int> mas)
{
	auto clock_start = chrono::high_resolution_clock::now();
	make_heap_pool(mas);

	for (int i = mas.size() - 1; i >= 0; i--)
	{
		int nextPosHeapItemsAmount;
		int posMaxTopElem = findPosMaxElem(mas, curState, i, nextPosHeapItemsAmount);
		if (posMaxTopElem != i)
		{
			swap(mas[i], mas[posMaxTopElem]);
			shiftDown(mas, nextPosHeapItemsAmount, posMaxTopElem);
		}
		PrevState(curState);
	}

	auto clock_stop = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::nanoseconds>(clock_stop - clock_start);
	cout << duration.count();
	return mas;
}

