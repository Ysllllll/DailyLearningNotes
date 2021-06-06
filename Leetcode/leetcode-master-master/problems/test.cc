#include "test.h"

#include <iostream>
#include <vector>
#include <bits/stdc++.h>
using namespace std;

int main()
{
	int bags = 0;

	cin >> bags;

	vector<int> arr;
	int n;

	while (cin >> n)
		arr.push_back(n);
	int sz = arr.size();
	vector<int> weights(arr.begin(), arr.begin() + sz / 2);
	vector<int> values(arr.begin() + sz / 2, arr.end());

	vector<int> dp(bags + 1, 0);
	for (int i = bags; i >= weights[0]; i++)
		dp[i] = dp[i - weights[0]] + values[0];

	for (int i = 1; i < weights.size(); i++)
		for (int j = bags; j >= weights[i]; j--)
			dp[j] = max(dp[j], dp[j - weights[i]] + values[i]);

	cout << dp[bags] << endl;

	return 0;
}

int main2()
{
	int N, x;
	cin >> N;
	vector<int> f(N + 1, 0);
	vector<int> arr, w, v;
	while (cin >> x)
	{
		arr.push_back(x);
	}
	int n = arr.size() / 2;
	w.assign(arr.begin(), arr.begin() + n);
	v.assign(arr.begin() + n, arr.end());
	for (int i = 0; i < n; ++i)
	{
		for (int j = N; j >= w[i]; --j)
		{
			f[j] = max(f[j], f[j - w[i]] + v[i]);
		}
	}
	cout << f[N] << endl;
	return 0;
}

int main3()
{

	return 0;
}