#include<iostream>
#include<fstream>
#include<string>
using namespace std;

void readFile(ifstream& fin, string* str)
{
	string temp;
	int i = 0;
	while (!fin.eof())
	{
		getline(fin, temp);
		str[i] = str[i] + temp;
		i++;
	}
}

void readDictionaryFile(ifstream& fin, string* str, int* t, int* p, int* n)
{
	string temp;
	int i = 0;
	while (fin >> temp)
	{
		fin >> t[i];
		fin >> p[i];
		fin >> n[i];

		str[i] = str[i] + temp;
		//cout << t[i] << endl;;
		i++;
	}
}

void writeFile(ofstream& out, string** str, int max)
{
	for (int i = 0; i < max; i++)
	{
		for (int j = 0; str[i][j] != "\0"; j++)
		{
			out << str[i][j] << endl;
		}
		//out << endl;
	}
}

void toWords(string* strSen, string** strWords, int max)
{
	string temp;

	for (int i = 0; i < max; i++)
		for (int k = 0, j = 0; j < strSen[i].length();k++, j++)
		{
			for (; strSen[i][j] != ' ' && strSen[i][j] != '\0';j++)
				temp = temp + strSen[i][j];

			strWords[i][k] = strWords[i][k] + temp + "";
			temp = "\0";
		}
}

bool isNotAlpha(char a)
{
	if (a >= 'a' && a <= 'z' || a >= 'A' && a <= 'Z' || a == ' ')
		return false;

	return true;
}

void nonAlpha(string**& words, int max, int max1)
{
	string** updated = new string * [max];

	for (int i = 0; i < max; i++)
		updated[i] = new string[max1];

	int li = 0, wo = 0, flag = 0;

	for (int i = 0; i < max; i++)
	{
		for (int j = 0; words[i][j].length(); j++)
		{
			for (int k = 0; words[i][j][k] != '\0'; k++)
				if (isNotAlpha(words[i][j][k]))
				{
					flag = 1;
					break;
				}

			if (flag == 0)
			{
				updated[i][wo] = updated[i][wo] + words[i][j] + "";
				wo++;
			}
			flag = 0;
		}

		wo = 0;
		li++;
	}

	words = updated;
}

void nonStopWords(string**& words, string* stop, int max, int max1)
{
	string** updated = new string * [max];
	for (int i = 0; i < max; i++)
		updated[i] = new string[max1];

	int li = 0, wo = 0, flag = 0;

	for (int i = 0; i < max; i++)
	{
		for (int j = 0; words[i][j] != "\0"; j++)
		{
			for (int k = 0; k < 201; k++)
				if (words[i][j] == stop[k])
				{
					flag = 1;
					break;
				}

			if (flag == 0)
			{
				updated[i][wo] = updated[i][wo] + words[i][j] + "";
				wo++;
			}

			flag = 0;
		}

		wo = 0;
		li++;
	}

	words = updated;
}

void checkSentimel(int p, int n)
{
	if (p > n)
		cout << "Tense is positive";
	else
		cout << "Tense is negative";

	cout << endl;
}

void checkInDictionary(string* line, string* dict, int* tot, int* pos, int* neg)
{
	int positive = 0, negative = 0, total = 0;

	float normPositive = 0.0, normNegative = 0.0;

	for (int i = 0; line[i] != "\0"; i++)
	{
		for (int j = 0; j < 216670; j++)
			if (line[i] == dict[j])
			{
				total += tot[j];
				positive += pos[j];
				negative += neg[j];

				normPositive = normPositive + ((pos[j] * 1.0) / tot[j]);
				normNegative = normNegative + ((neg[j] * 1.0) / tot[j]);

				break;
			}
	}

	cout << "Without normilization:\n\n";
	cout << "Positive: " << positive << "\nNegative: " << negative << endl << endl;
	checkSentimel(positive, negative);

	cout << "\nTotal: " << total << endl;
	cout << "\nWith normilization:\n\n";
	cout << "Positive: " << normPositive << "\nNegative: " << normNegative << endl << endl;
	checkSentimel(normPositive, normNegative);
}

int main()
{
	int totalLines = 1, totalStrLines = 1000, wordLength = 50;

	int* total = new int[216670], * positive = new int[216670], * negative = new int[216670];

	string* stop = new string[210], * dict = new string[216670];

	string* lines = new string[totalStrLines];

	string** words = new string * [totalStrLines];
	for (int i = 0; i < totalStrLines; i++)
		words[i] = new string[wordLength];

	ifstream fin("rawdata.txt");
	readFile(fin, lines);

	ifstream fin1("stopwords.txt");
	readFile(fin1, stop);

	ifstream fin2("CountDictionary.txt");
	readDictionaryFile(fin2, dict, total, positive, negative);

	int user;
	cout << "Enter 1 to input sentence or 2 to run default = ";
	cin >> user;

	string* temp = new string{ "kid like to save those cutee3 squirrels and have popsicle" };

	if (user == 1)
	{
		cout << "Enter sentence = ";
		cin.ignore();
		getline(cin, *temp);
	}
	cout << endl;

	toWords(temp, words, totalLines);
	nonAlpha(words, totalLines, wordLength);
	nonStopWords(words, stop, totalLines, wordLength);

	ofstream fout("data.txt");
	writeFile(fout, words, totalLines);

	checkInDictionary(words[0], dict, total, positive, negative);

	cout << endl << "Final words: ";
	for (int i = 0; i < totalLines; i++)
	{
		for (int j = 0; j < words[i][j].length(); j++)
			cout << words[i][j] << ' ';

		cout << endl;
	}

	return 0;
}