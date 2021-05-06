#include "myRankInterface.h"
#include <iostream>
#include <string>
#include <random>
#include <ctime>

using namespace std;

void testcase(IrankList<string, int>*);

int main()
{
    IrankList<string, int>* p_list = createRankList<string, int>();

    string str_key;
    int n_score;
    int n_branch;
    vector<string> res;

    testcase(p_list);

    do
	{
		cout << endl;
		cout << "Please input to select: " << endl;
		cout << "0) Insert(name, score)" << "\t\t";
		cout << "1) Delete(name)" << "\t\t";
		cout << "2) Update(name, score)" << "\t\t" << endl;
		cout << "3) Query detail(name)" << "\t\t";
		cout << "4) Query role(rank)" << "\t\t";
		cout << "5) Query top 10" << "\t\t" << endl;
		cout << "6) Exit" << "\t\t" << endl;
		cin >> n_branch;
		
		switch (n_branch)
		{
		case 0:
			cout << "Role name: " << endl;
			cin >> str_key;
			cout << "Role score: " << endl;
			cin >> n_score;
			if (p_list->insert(str_key, n_score)) {
                cout << "Insert success" << endl;
            }
			else {
                cout << "Insert fail" << endl;
            }
			break;
		case 1:
			cout << "Role name: " << endl;
			cin >> str_key;
			if (p_list->remove(str_key)) {
                cout << "Delete success" << endl;
            }
            else {
                cout << "Delete fail" << endl;
            }
			break;
		case 2:
            cout << "Role name: " << endl;
			cin >> str_key;
            cout << "New score: " << endl;
			cin >> n_score;
            if (p_list->update(str_key, n_score)) {
                cout << "Update success" << endl;
            }
            else {
                cout << "Update fail" << endl;
            }
			break;
		case 3:
			cout << "Role name: " << endl;
			cin >> str_key;
            cout << p_list->find(str_key) << "\t" << str_key << ": " << p_list->value(str_key) << endl;
			break;
		case 4:
			cout << "Rank number: " << endl;
			cin >> n_score;
            res = p_list->range(n_score, n_score + 4);
            for (string &s: res) {
                cout << n_score++ << "\t" << s << ": " << p_list->value(s) << endl;
            }
			break;
		case 5:
			res = p_list->top();
            for (int i = 0; i < TOP_NUMBER; i++) {
                cout << i+1 << "\t" << res[i] << ": " << p_list->value(res[i]) << endl;
            }
			break;
		case 6:
			exit(0);
			break;
		default:
			cout << "Wrong input! Try again!" << endl;
			break;
		}
	} while (n_branch != 6);

    return 0;
}

void testcase(IrankList<string, int>* p_list)
{
    srand(time(NULL));

    for (int i = 0; i < 26; i++) {
        char tmp = 'a' + i;
        string role(1, tmp);
        int score = rand() % 1000;
        p_list->insert(role, score);
    }
}