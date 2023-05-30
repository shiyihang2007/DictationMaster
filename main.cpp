// Think twice, code once.
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <tuple>
#include <vector>

#include "yaml-cpp/node/node.h"
#include "yaml-cpp/node/parse.h"
#include "yaml-cpp/yaml.h"

using namespace std;

mt19937 gen(chrono::system_clock::now().time_since_epoch().count());
void systemClear() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

string infoUsage;
string infoChooseMode;
string infoChooseDict;
string infoInputValue;
string infoErrorDictLine;
string infoPreview;
string infoPause;
string infoEndup;

void loadInfo() {
    YAML::Node infoIn;
    string infoYaml =
#ifdef ANSI
        "../plain/info_ansi.yaml"
#else
        "../plain/info_utf_8.yaml"
#endif
        ;
    infoIn = YAML::LoadFile(infoYaml);
    infoUsage = infoIn["Usage"].as<string>();
    infoChooseMode = infoIn["ChooseMode"].as<string>();
    infoChooseDict = infoIn["ChooseDict"].as<string>();
    infoInputValue = infoIn["InputValue"].as<string>();
    infoErrorDictLine = infoIn["ErrorDictLine"].as<string>();
    infoPreview = infoIn["Preview"].as<string>();
    infoPause = infoIn["Pause"].as<string>();
    infoEndup = infoIn["Endup"].as<string>();
}

namespace Practice {
    int st, acval, waval, num, ac, lst = -1;
    string dictionary;
    vector<tuple<string, string, int>> v;

    void loadInfo();
    void loadDict();
    void judge(const string & /*s*/, int /*pos*/);
    void getPos(int & /*pos*/, int /*sum*/);
    void main();
} // namespace Practice

namespace Dictation {
    vector<string> res;
    vector<int> wrong;
    vector<pair<string, string>> v;
    string dictionary;

    void main();
} // namespace Dictation

auto main() -> int {
#ifdef _WIN32
    system("chcp 65001");
    system("cls");
#endif
    loadInfo();
    cout << infoUsage << endl;
    cin.get();
    systemClear();
    cout << infoChooseMode << endl;
    int x;
    cin >> x;
    systemClear();
    if (x == 1) {
        Practice::main();
    }
    else {
        Dictation::main();
    }
    return 0;
}

void Practice::loadDict() {
    ifstream din(dictionary.c_str());
    string s;
    for (int line = 1; getline(din, s); line++) {
        if (s == "\n" || s == "\r" || s == "\r\n") {
            continue;
        }
        int pos = -1;
        for (int i = 0; i < static_cast<int>(s.length()); i++) {
            if (s[i] == ':') {
                pos = i;
                break;
            }
        }
        if (pos == -1) {
            systemClear();
            cout << infoErrorDictLine << line << endl;
            return;
        }
        v.emplace_back(s.substr(0, pos - 1), s.substr(pos + 2), st);
    }
}

void Practice::getPos(int &pos, int sum) {
    int rnd = static_cast<int>(gen()) % sum + 1;
    int tmp = 0;
    for (int i = 0; i < static_cast<int>(v.size()); i++) {
        if (rnd <= tmp + get<2>(v[i])) {
            pos = i;
            break;
        }
        tmp += get<2>(v[i]);
    }
}

void Practice::judge(const string &s, int pos) {

    if (s == get<0>(v[pos])) {
        ac++;
        get<2>(v[pos]) = max(get<2>(v[pos]) - acval, 0);
        cout << "Accepted." << endl;
    }
    else if (s == get<0>(v[pos]) + " /pass") {
        ac += get<2>(v[pos]);
        num += get<2>(v[pos]) - 1;
        get<2>(v[pos]) = 0;
        cout << "Accepted and passed." << endl;
    }
    else if (s == "/skip") {
        num--;
    }
    else if (s + "." == get<0>(v[pos])) {
        cout << "You forget the '.'! /cf/cf/cf" << endl;
        ac++;
    }
    else {
        get<2>(v[pos]) += waval;
        cout << "Wrong. The answer is:";
        cout << get<0>(v[pos]).c_str() << endl;
    }
}

void Practice::main() {
    cout << infoChooseDict << endl;
    cin >> dictionary;
    cout << infoInputValue << endl;
    cin >> st >> acval >> waval;
    systemClear();
    loadDict();
    cout << infoPreview << endl;
    for (auto i : v) {
        cout << get<1>(i).c_str() << " : ";
        cout << get<0>(i).c_str() << endl;
    }
    cout << infoPause << endl;
    cin.get();
    cin.get();
    while (1) {
        int sum = 0;
        for (auto i : v) {
            sum += get<2>(i);
        }
        if (sum == 0) {
            break;
        }
        int pos = -1;
        getPos(pos, sum);
        if (pos == lst && get<2>(v[pos]) != sum) {
            continue;
        }
        lst = pos;
        systemClear();
        cout << get<1>(v[pos]).c_str();
        cout << " (value: " << get<2>(v[pos]);
        cout << ", sum: " << sum;
        string s;
        do {
            getline(cin, s);
        } while (s == "\n" || s == "\r" || s == "\r\n");
        cout << endl;
        while (s.back() == ' ') {
            s.pop_back();
        }
        if (s == "/finish") {
            break;
        }
        judge(s, pos);
        num++;
        cout << infoPause << endl;
        getchar();
    }
    systemClear();
    cout << infoEndup << endl;
    cout << "All: " << num << endl;
    cout << "AC: " << ac << endl;
    cout << "ACC: " << static_cast<double>(ac) / num * 100.0 << endl;
    cout << infoPause << endl;
    cin.get();
}

void Dictation::main() {
    puts("请选择词库");
    cin >> dictionary;
    ifstream din(dictionary.c_str());
    {
        string s;
        for (int line = 1; getline(din, s); line++) {
            if (s == "\n" || s == "\r" || s == "\r\n") {
                continue;
            }
            int pos = -1;
            for (int i = 0; i < static_cast<int>(s.length()); i++) {
                if (s[i] == ':') {
                    pos = i;
                    break;
                }
            }
            if (pos == -1) {
                systemClear();
                printf("词典第 %d 行出错。\n程序停止运行。\n", line);
                return;
            }
            v.emplace_back(s.substr(0, pos - 1), s.substr(pos + 2));
        }
    }
    systemClear();
    puts("词典加载成功。");
    puts("==预览==");
    for (const auto &i : v) {
        printf("%s : %s\n", i.second.c_str(), i.first.c_str());
    }
    puts("输入回车开始默写。");
    cin.get();
    cin.get();
    for (const auto &i : v) {
        systemClear();
        printf("%s\n", i.second.c_str());
        string s;
        do {
            getline(cin, s);
        } while (s == "\n" || s == "\r" || s == "\r\n");
        res.push_back(s);
        systemClear();
    }
    int ac = 0;
    int num = res.size();
    for (int i = 0; i < static_cast<int>(res.size()); i++) {
        if (res[i] == v[i].first) {
            ac++;
        }
        else {
            wrong.push_back(i);
        }
    }
    printf("你的得分为：%dpts.\n",
           static_cast<int>(floor(static_cast<double>(ac) / num * 100)));
    if (ac != num) {
        puts("你错误的题目：");
        for (auto i : wrong) {
            printf("%s\n你的答案为：%s\n正确答案为：%s\n\n",
                   v[i].second.c_str(), res[i].c_str(), v[i].first.c_str());
        }
    }
    else {
        puts("膜拜 AK King! /bx/bx/bx");
    }
}
