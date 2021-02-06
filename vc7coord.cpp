#include <iostream>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iomanip>
using namespace std;

string guards("ABCDEFG");
long double radius(1.0);

void checkEntireArray(string points[], long double x[], long double y[], int size, long double r);
bool AShouldNotSeeB(string a, string b, string middlePoints[], long double Ax, long double Ay, long double Bx, long double By, long double x[], long double y[], int size, long double r);
bool ASeesB(string a, string b, string middlePoints[], long double Ax, long double Ay, long double Bx, long double By, long double x[], long double y[], int size, long double r);
bool ASeesBWithoutCBlocking(long double Ax, long double Ay, long double Bx, long double By, long double Cx, long double Cy, long double r);
long double distance(long double Ax, long double Ay, long double Bx, long double By);
void printInput(string points[], long double x[], long double y[], int size);
void mxplusb(long double Ax, long double Ay, long double Bx, long double By);
void checkMissing(string points[], int size);

int main()
{
	fstream f;
	string value;
	string filename = "./input.txt";
	
	ifstream myFile;
	string line;
	int lines;
	myFile.open(filename);
	for(lines = 0; getline(myFile,line); lines++);
	myFile.close();

	int size = lines;
	string* points = new string[size];
	long double* x = new long double[size];
	long double* y = new long double[size];

	f.open(filename.c_str());
	int i = 0;
	int j = 0;
	int k = 0;
	while (f >> value)
	{
		if (i % 3 == 0)
		{
			points[j] = value;
		}
		if (i % 3 == 1)
		{
			x[j] = atof(value.c_str());
		}
		if (i % 3 == 2)
		{
			y[j] = atof(value.c_str());
			j++;
		}
		i++;
	}
	cout << showpoint;
	cout << setprecision(12);
	printInput(points, x, y, size);
	checkMissing(points, size);
	
	checkEntireArray(points, x, y, size, radius);
	return 0;
}

void checkEntireArray(string points[], long double x[], long double y[], int size, long double r)
{
	string allGuards = guards;
	int start;
	int end;
	int subarray_size;
	int error = 0;
	string s = "";
	for (int i = 0; i < size; ++i)
	{
		if (points[i].length() > 1) // viewpoint
		{
			for (int j = 0; j < allGuards.length(); ++j)
			{
				if (strchr(points[i].c_str(), allGuards.at(j))) // this viewpoint sees allGuards.at(j)
				{
					for (int k = 0; k < size; ++k)
					{
						s = "";
						s += allGuards.at(j);
						if (points[k].compare(s) == 0) // k is the guard, i is the viewpoint
						{
							if (k > i)
							{
								start = i;
								end = k;
							}
							else
							{
								start = k;
								end = i;
							}
						}
					}
					subarray_size = end - start - 1;
					// if adjacent, only check if too far away
					if (subarray_size == 0)
					{
						if (distance(x[start], y[start], x[end], y[end]) > r)
						{
							cout << "Viewpoint " << points[i] << " is adjacent to guard " << allGuards.at(j) << " and is suppose to see this guard but too far away. " << endl; 
							error++;
						}
					}
					// extract entire points[0...n-1] to subarray[start...end]
					else if (subarray_size > 0)
					{
						string* subPoints = new string[subarray_size];
						long double* subX = new long double[subarray_size];
						long double* subY = new long double[subarray_size];
						for (int l = 0; l < subarray_size; ++l)
						{
							subPoints[l] = points[start + 1 + l];
							subX[l] = x[start + 1 + l];
							subY[l] = y[start + 1 + l];
						}
						if (!ASeesB(points[start], points[end], subPoints, x[start], y[start], x[end], y[end], subX, subY, subarray_size, r))
						{
							error++;
						}
					}
					else
					{
						cout << "Subarray size exception. " << endl;
					}
				}
				else // this viewpoint does not see allGuards.at(j)
				{

					for (int k = 0; k < size; ++k)
					{
						s = "";
						s += allGuards.at(j);
						if (points[k].compare(s) == 0)
						{
							if (k > i)
							{
								start = i;
								end = k;
							}
							else
							{
								start = k;
								end = i;
							}
						}
					}
					subarray_size = end - start - 1;
					// if adjacent, only check if too far away
					if (subarray_size == 0)
					{
						if (distance(x[start], y[start], x[end], y[end]) <= r)
						{
							cout << "Viewpoint " << points[i] << " is adjacent to guard " << allGuards.at(j) << " and not suppose to see this guard but sees it. " << endl; 
							error++;
						}
					}
					// extract entire points[0...n-1] to subarray[k...i]
					else if (subarray_size > 0)
					{
						string* subPoints = new string[subarray_size];
						long double* subX = new long double[subarray_size];
						long double* subY = new long double[subarray_size];
						for (int l = 0; l < subarray_size; ++l)
						{
							subPoints[l] = points[start + 1 + l];
							subX[l] = x[start + 1 + l];
							subY[l] = y[start + 1 + l];
						}
						if (!AShouldNotSeeB(points[start], points[end], subPoints, x[start], y[start], x[end], y[end], subX, subY, subarray_size, r))
						{
							error++;
						}
					}
					else
					{
						cout << "Subarray size exception. " << endl;
					}
				}
			}
		}
	}
}

bool AShouldNotSeeB(string a, string b, string middlePoints[], long double Ax, long double Ay, long double Bx, long double By, long double x[], long double y[], int size, long double r)
{
	bool flag = false;
	if (distance(Ax, Ay, Bx, By) > r)
	{
		return true; 
	}
	for (int i = 0; i < size; ++i)
	{
		if (!ASeesBWithoutCBlocking(Ax, Ay, Bx, By, x[i], y[i], r))
		{
			flag = true;
		}
	}
	if (flag == false)
	{
		if (a.length() == 1)
		{
			cout << b << "(" << Bx << ", " << By << ") sees " << a << "(" << Ax << ", " << Ay << ") but not suppose to see it. " << endl;
		}
		if (b.length() == 1)
		{
			cout << a << "(" << Ax << ", " << Ay << ") sees " << b << "(" << Bx << ", " << By << ") but not suppose to see it. " << endl;
		}
		
	}	
	return flag;
}

bool ASeesB(string a, string b, string middlePoints[], long double Ax, long double Ay, long double Bx, long double By, long double x[], long double y[], int size, long double r)
{
	if (distance(Ax, Ay, Bx, By) > r)
	{
		cout << a << "(" << Ax << ", " << Ay << ") and " << b << "(" << Bx << ", " << By << ") are too far away. " << endl;
		return false;
	}
	for (int i = 0; i < size; ++i)
	{
		if (!ASeesBWithoutCBlocking(Ax, Ay, Bx, By, x[i], y[i], r))
		{
			if (a.length() == 1)
			{
				cout << a << "(" << Ax << ", " << Ay << ") does not see " << b << "(" << Bx << ", " << By << ") but suppose to see it, blocking by " << middlePoints[i] << " (" << x[i] << ", " << y[i] << "). " << endl;
			}
			if (b.length() == 1)
			{
				cout << b << "(" << Bx << ", " << By << ") does not see " << a << "(" << Ax << ", " << Ay << ") but suppose to see it, blocking by " << middlePoints[i] << " (" << x[i] << ", " << y[i] << "). " << endl;
			}
			return false;
		}
	}
	return true;
}


bool ASeesBWithoutCBlocking(long double Ax, long double Ay, long double Bx, long double By, long double Cx, long double Cy, long double r)
{
	long double m = (Ay - By) / (Ax - Bx);
	long double b = Ay - m * Ax;
	long double y = m * Cx + b;
	return (y >= Cy);
}

long double distance(long double Ax, long double Ay, long double Bx, long double By)
{
	return sqrt(pow((Bx - Ax), 2) + pow((By - Ay), 2));
}

void printInput(string points[], long double x[], long double y[], int size)
{
	for (int i = 0; i < size; ++i)
	{
		cout << points[i] << "(" << x[i] << ", " << y[i] << ")" << endl;
	}
}

void mxplusb(long double Ax, long double Ay, long double Bx, long double By)
{

	long double m = (Ay - By) / (Ax - Bx);
	long double b = Ay - m * Ax;
	cout << "m = " << m << endl;
	cout << "b = " << b << endl;
}

void checkMissing(string points[], int size)
{
	string allPoints[120]; 
	int pivot = 0;
	bool flag;
	int count = 0;
	string str;
	for (int i_a = 0; i_a < 2; ++i_a)
	{
		for (int i_b = 0; i_b < 2; ++i_b)
		{
			for (int i_c = 0; i_c < 2; ++i_c)
			{
				for (int i_d = 0; i_d < 2; ++i_d)
				{
					for (int i_e = 0; i_e < 2; ++i_e)
					{
						for (int i_f = 0; i_f < 2; ++i_f)
						{
							for (int i_g = 0; i_g < 2; ++i_g)
							{
								str.clear();
								if (i_a == 1) { str.append("A"); }
		 						if (i_b == 1) { str.append("B"); }
								if (i_c == 1) { str.append("C"); }
		 						if (i_d == 1) { str.append("D"); }
								if (i_e == 1) { str.append("E"); }
		 						if (i_f == 1) { str.append("F"); }
								if (i_g == 1) { str.append("G"); }
								if (str.length() > 1) { 
									allPoints[pivot] = str; 
									pivot++;
								}
							}
						}
					}
				}
			}
		}
	}
	
	for (int i = 0; i < 120; ++i)
	{
		flag = false;
		for (int j = 0; j < size; ++j)
		{
			if (allPoints[i].compare(points[j]) == 0) { flag = true; }
		}
		if (!flag) 
		{
			cout << allPoints[i] << " ";
			count++;
		}
	}
	cout << "Missing viewpoints =  " << count << endl;

}

