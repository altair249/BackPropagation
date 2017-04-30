#include <vector>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include <sstream>
using namespace std;

class neural
{
public:
	neural(int NT, int MT, int JT, double etaT, double allowableErrorT, int maxEpochT, vector<double> xT, vector<double> tT);
	neural();
	vector<double> t, x;

	vector<vector<vector<double> > > w;
	vector<double> error;
	vector<vector<double> > outY;
	double allowableError;
	int N, M, J, maxEpoch;
	double eta;

	double fa(double net);
	double dfa(double net);
	bool teaching();
	void resetW();
};

class out
{
public:
	string fullInfoTeaching();
	out();

private:
	string vectorToStr(vector<double> x);
	double roundPrecision(double x);
	neural Net;
	vector<double> x;
	vector<double> t;
};



neural::neural(int NT, int MT, int JT, double etaT, double allowableErrorT, int maxEpochT, vector<double> xT, vector<double> tT)
{
	N = NT + 1;
	M = MT;
	J = JT + 1;
	eta = etaT;
	maxEpoch = maxEpochT;
	allowableError = allowableErrorT;
	x = xT;
	t = tT;
}

neural::neural()
{
	N = 0;
	M = 0;
	J = 0;
}

bool neural::teaching()
{
	resetW();
	double rmsError;

	for (int epoch = 0; epoch <= maxEpoch; ++epoch)
	{
		vector<double> outHidden;					// Auoia ne?uoiai neiy eee aoia iineaaiaai neiy
		outY.push_back(vector<double>());		// Auoia auoiaiiai neiy eee y
		vector<vector<double> > delta;				// delta aey ea?aiai neiy
		delta.push_back(vector<double>());          // aey ne?uoiai
		delta.push_back(vector<double>());          // aey auoiaiiai
		rmsError = 0;

		// ?an?ao auoiaa aey ne?uoiai neiy
		for (int j = 0; j < J; ++j)
		{
			double net = w[0][0][j];
			for (int i = 1; i < N; ++i)
			{
				net += w[0][i][j] * x[i];
			}
			double outNet = fa(net);
			outHidden.push_back(outNet);
		}

		// ?an?ao auoiaa aey auoiaiiai neiy
		for (int j = 0; j < M; ++j)
		{
			double net = w[1][0][j];
			for (int i = 1; i < J; ++i)
			{
				net += w[1][i][j] * outHidden[i];
			}
			double y = fa(net);
			outY[epoch].push_back(y);

			// Iian?ao aaeuo aey auoiaiiai neiy
			delta[1].push_back(dfa(net) * (t[j] - y));
			rmsError += pow(t[j] - y, 2);
		}

		// Iian?ao aaeuo aey ne?uoiai neiy
		for (int j = 0; j < J; ++j)
		{
			double net = w[0][0][j];
			for (int i = 1; i < N; ++i)
			{
				net += w[0][i][j] * x[i];
			}
			double wDelta = 0;
			for (int i = 0; i < M; ++i)
			{
				wDelta += w[1][j][i] * delta[1][i];
			}
			delta[0].push_back(dfa(net) * wDelta);
		}

		for (int j = 0; j < J; ++j)
		{
			// ei??aeoe?iaea aania ne?uoiai neiy
			for (int i = 1; i < N; ++i)
				w[0][i][j] += eta * x[i] * delta[0][j];
		}

		for (int j = 0; j < M; ++j)
		{
			//ei??aeoe?iaea aania auoiaiiai neiy
			for (int i = 1; i < J; ++i)
				w[1][i][j] += eta * outHidden[i] * delta[1][j];
		}

		rmsError = sqrt(rmsError);
		error.push_back(rmsError);

		if (rmsError < allowableError)
			return false;
	}
	return true;
}

void neural::resetW()
{
	w = vector<vector<vector<double> > >(2, vector<vector<double>>());
	for (int i = 0; i < N; ++i) // Aiaaaeyai ana aana ne?uoiai neiy 
		w[0].push_back(vector<double>(J, 0));

	for (int i = 0; i < J; ++i) // Aiaaaeyai ana aana iineaaiaai neiy 
		w[1].push_back(vector<double>(M, 0));

	for (int k = 0; k<2; ++k)
	{
		for (int i = 0; i < w[k].size(); ++i)
			for (int j = 0; j < w[k][i].size(); ++j) // onoaiiaei aana niauaiey neo?aeiuie cia?aieyie [0; 1) 
				w[k][i][j] = ((double)rand() / (RAND_MAX));
	}
}

double neural::fa(double net)
{
	return (1 - exp(net)) / (1 + exp(net));
}

double neural::dfa(double net)
{
	return (1 - pow(fa(net), 2)) / 2;
}

out::out()
{
	x = vector<double>(2, 0);
	t = vector<double>(1, 0.4);
	x[0] = 1;
	x[1] = 2;
	Net = neural(1, 1, 2, 1.0, 0.001, 500, x, t);
}

double out::roundPrecision(double x)
{
	return x < 0.0 ? ceil(x * pow(10, 5) - 0.5) / pow(10, 5) : floor(x * pow(10, 5) + 0.5) / pow(10, 5);
}

string out::vectorToStr(vector<double> x)
{
	std::stringstream res;
	res << "(";
	for (int i = 0; i < x.size() - 1; ++i)
	{
		res << roundPrecision(x[i]) << ", ";
	}
	res << roundPrecision(x.back()) << ")";
	return res.str();
}

string out::fullInfoTeaching()
{
	Net.teaching();
	std::stringstream res;
	res << "Iao?aiea iae?iiiie naoe i?ienoiaeei " << Net.error.size() << " yiio\n";
	res << "A eoiaa n?aaiaeaaa?aoe?iay iia?aoiinou noaea ?aaia " << Net.error.back() << ".\n";
	res << "\nOaae iao?aiey:\n";
	for (int i = 0; i < Net.error.size(); ++i)
	{
		res << "Yiioa " << i << ": y = " << vectorToStr(Net.outY[i]) << ", E(" << i << ") = " << roundPrecision(Net.error[i]) << "\r\n";
		res << "Aana: W(ne?uoue neie) = (\n";
		for (int i = 0; i < Net.N; ++i)
		{
			res << "\t{";
			for (int j = 0; j < Net.J - 1; ++j)
				res << Net.w[0][i][j] << ";";
			res << Net.w[0][i].back();
			res << "},\n";
		}
		res << ")\nAana: W(auoiaiie neie) = (\n";
		for (int i = 0; i < Net.J; ++i)
		{
			res << "\t{";
			for (int j = 0; j < Net.M - 1; ++j)
				res << Net.w[1][i][j] << ";";
			res << Net.w[1][i].back();
			res << "},\n";
		}
	}
	res << ")\n";
	return res.str();
}

int main()
{
	setlocale(LC_ALL, "Russian");

	// Создание объекта класса нейронной сети и передача его в класс вывода
	out res;

	bool exit = 0;			// Завершения работы программы, когда exit == 1
	do
	{
		cout << "Выберите часть лабораторной 3:";
		cout << "\n0 - выход;";
		cout << "\n1 - запустить обучение;\n";

		int userAns;
		cin >> userAns;
		switch (userAns)
		{
		case 0:
			cout << "\nGoobay!";
			exit = 1;				// Выход из программы
			break;
		case 1:
			cout << res.fullInfoTeaching();
			break;
		default:
			cout << "\nПовторите ввод.";// Что-то не то
			break;
		}
	} while (!exit);


	return 0;
}
