#include <iostream>
#include <clocale>
using namespace std;

class Equation
{
private:

	double *a{}, *x{}, y{};

public:

	Equation(int N)
	{
		a = new double[N];
		x = new double[N];
	}

	~Equation()
	{
		delete[] a;
		delete[] x;
	}

	friend class System;
};

class System
{
private:

	Equation** R; // массив указателей на адреса уравнений
	int N; // количество уравнений
	bool flag = 0; //переменная, определяющая совместность или несовместность системы

public:

	System (int n)
	{
		N = n;
		R = new Equation*[n];
		for (int i = 0; i < n; i++)
			R[i] = new Equation(n);
	}

	void Input() {
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				cout << "a[" << (i + 1) << "][" << (j + 1) << "]= ";
				cin >> R[i]->a[j];
			}
		}
		for (int i = 0; i < N; i++)
		{
			cout << "y[" << (i + 1) << "]= ";
			cin >> R[i]->y;
		}
		cout << "-------------------------------------------------------------------" << endl;
	}

	void Output()
	{
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				cout << R[i]->a[j] << "*x" << (j + 1);
				if (j < N - 1)
					cout << " + ";
			}
			cout << " = " << R[i]->y << endl;
		}
	}

	void Gauss(double eps)
	{
		double max;
		int k, index;
		k = 0;
		while (k < N)
		{
			// Поиск строки с максимальным a[i][k]
			max = abs(R[k]->a[k]);
			index = k;
			for (int i = k + 1; i < N; i++)
			{
				if (abs(R[i]->a[k]) > max)
				{
					max = abs(R[i]->a[k]);
					index = i;
				}
			}

			if (max < eps) 	// нет ненулевых диагональных элементов
			{
				flag = 1;
				cout << "Система несовместна" << endl;
				cout << "Решение получить невозможно из-за нулевого столбца ";
				cout << (index + 1) << " матрицы Alpha" << endl;
				break;
			}
			for (int j = 0; j < N; j++)  // Перестановка строк
			{
				double temp = R[k]->a[j];
				R[k]->a[j] = R[index]->a[j];
				R[index]->a[j] = temp;
			}
			double temp = R[k]->y;
			R[k]->y = R[index]->y;
			R[index]->y = temp;
			// Нормализация уравнений
			for (int i = k; i < N; i++)
			{
				double temp = R[i]->a[k];
				if (abs(temp) < eps) continue; // для нулевого коэффициента пропустить
				for (int j = 0; j < N; j++)
					R[i]->a[j] = R[i]->a[j] / temp;
				R[i]->y = R[i]->y / temp;
				if (i == k)  continue; // уравнение не вычитать само из себя
				for (int j = 0; j < N; j++)
					R[i]->a[j] = R[i]->a[j] - R[k]->a[j];
				R[i]->y = R[i]->y - R[k]->y;
			}
			k++;
		}
		// обратная подстановка
		for (k = N - 1; k >= 0; k--)
		{
			R[k]->x[k] = R[k]->y;
			for (int i = 0; i < k; i++)
				R[i]->y = R[i]->y - R[i]->a[k] * R[k]->x[k];
		}
	}

	void Solution() {
		if (flag == 0) {
			for (int i = 0; i < N; i++)
				cout << "x[" << (i + 1) << "]=" << R[i]->x[i] << endl;
		}
	}

	~System()
	{
		for (int i = 0; i < N; i++)
			delete R[i];
		delete[] R;
	}

};

int Enter_n();
double Enter_eps();

int main()
{
	setlocale(LC_ALL, "RUS");
	system("cls");
	int n = Enter_n();
	double eps = Enter_eps();    //Точность
	System Alpha(n);
	Alpha.Input();
	Alpha.Output();
	Alpha.Gauss(eps);
	Alpha.Solution();
	return 0;
}

int Enter_n() 
{
	int _n;
	do {
		cout << "Введите количество уравнений" << endl;
		cin >> _n;
	} while ((_n < 0) || (_n == 0));
	return (_n);
}

double Enter_eps()
{
	double _eps;
	do {
		cout << "Введите точность вычисления корней" << endl;
		cin >> _eps;
	} while ((_eps < 0) || (_eps == 0));
	return (_eps);
}