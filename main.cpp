#include <iostream>
#include <vector>
#include <iomanip>
#include <stdio.h>
#include <algorithm>
#include "json.hpp"
using json = nlohmann::json;
using namespace std;

class Holes
{
public:
    string name;
    string object;
};

class User_Type
{
public:
    string naming;
    string construction_type;
    string elem_type;
    string param;
    vector <string> types;
};

vector <vector<int>> itog;
vector <string> Typesuni;
bool Is_us_array(string Data, string ty, vector<User_Type> us_types, vector<string> spisok);
bool Is_structure(json Data, string structur);
bool Is_us_union(vector<string> spisok, User_Type unions);
bool Is_us_intersection(vector<string> spisok, User_Type intersects);



vector<int> Matrix(int** check, int i1, vector<int> fcf, int n)
{
    vector<int> fgh = fcf;
    if (fcf.size() == n) itog.push_back(fgh);
    vector<int> copy;
    if (i1 <= n - 1)
    {
        for (int i = i1; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                int bv = 0;
                for (int r = 0; r < fgh.size(); r++)
                {
                    if (j == fgh[r]) bv = 1;
                }
                if (bv == 0)
                {
                    if (check[i][j] == 1)
                    {
                        fgh.push_back(j);
                        copy = Matrix(check, i + 1, fgh, n);
                        fgh.pop_back();
                    }
                }
            }
        }
    }
    return copy;
}


bool Is_true_array(string Data, string ty, vector<User_Type> us_types, vector<string> spisok)
{
    json Doc{ json::parse(Data) };
    int k = 0;
    int r = 0;

    if (ty == "integer")
    {
        r = 1;
        for (auto& Item : Doc)
        {
            if (Item.is_number_integer() == false) k = 1;
        }
        if (k == 0) return true;
    }
    if (ty == "null")
    {
        r = 1;
        for (auto& Item : Doc)
        {
            if (Item.is_null() == false) k = 1;
        }
        if (k == 0) return true;
    }
    if (ty == "real")
    {
        r = 1;
        for (auto& Item : Doc)
        {
            if (Item.is_number_float() == false) k = 1;
        }
        if (k == 0) return true;
    }
    if (ty == "string")
    {
        r = 1;
        for (auto& Item : Doc)
        {
            if (Item.is_string() == false) k = 1;
        }
        if (k == 0) return true;
    }
    if (ty == "object")
    {
        r = 1;
        for (auto& Item : Doc)
        {
            if (Item.is_object() == false) k = 1;
        }
        if (k == 0) return true;
    }
    
    if (r == 0)
    {
        for (int i = 0; i < us_types.size(); i++)
        {
            if (ty == us_types[i].naming)
            {
                if (us_types[i].construction_type == "Array")
                {
                    for (auto& Item : Doc)
                    {
                        std::string str{ Item.dump() };
                        if (Is_us_array(str, us_types[i].elem_type, us_types, spisok) == false) k = 1;
                    }
                    if (k == 0) return true;
                }
                if (us_types[i].construction_type == "Union")
                {
                    for (auto& Item : Doc)
                    {
                        vector<string> spisok1 = spisok;
                        if (Item.is_number_integer() == true) spisok1.push_back("integer");
                        if (Item.is_number_float() == true) spisok1.push_back("real");
                        if (Item.is_null() == true) spisok1.push_back("null");
                        if (Item.is_string() == true) spisok1.push_back("string");
                        if (Item.is_object() == true) spisok1.push_back("object");
                        if (Is_us_union(spisok1, us_types[i]) == false) k = 1;
                    }
                    if (k == 0) return true;
                }
                if (us_types[i].construction_type == "Intersection")
                {
                    for (auto& Item : Doc)
                    {
                        vector<string> spisok1 = spisok;
                        if (Item.is_number_integer() == true) spisok1.push_back("integer");
                        if (Item.is_number_float() == true) spisok1.push_back("real");
                        if (Item.is_null() == true) spisok1.push_back("null");
                        if (Item.is_string() == true) spisok1.push_back("string");
                        if (Item.is_object() == true) spisok1.push_back("object");
                        if (Is_us_intersection(spisok, us_types[i]) == false) k = 1;
                    }
                    if (k == 0) return true;
                }
                if (us_types[i].construction_type == "Struct")
                {
                    for (auto& Item : Doc)
                    {
                        if (Is_structure(Item, us_types[i].param) == false) k = 1;
                    }
                    if (k == 0) return true;
                }
                if (us_types[i].construction_type == "Unification")
                {
                    for (auto& Item : Doc)
                    {
                        for (int r = 0; r < us_types[i].types.size(); r++)
                        {
                            if (Is_structure(Item, us_types[i].types[r]) == true) k = 1;
                        }
                        if (k == 0) return false;
                    }
                    return true;
                }
            }
        }
    }
    return false;
}


bool Is_us_array(string Data, string ty, vector<User_Type> us_types, vector<string> spisok)
{
    json Doc{ json::parse(Data) };
    std::string Data1{ Doc.dump() };
    if (Data1[0] == '[')
    {
        if (Is_true_array(Data1, ty, us_types, spisok) == true)  return true;
        else return false;
    }

    return false;
}




bool Is_us_union(vector<string> spisok, User_Type unions)
{
    for (int i = 0; i < spisok.size(); i++)
    {
        for (int j = 0; j < unions.types.size(); j++)
        {
            if (spisok[i] == unions.types[j]) return true;
        }
    }
    return false;
}

bool Is_us_intersection(vector<string> spisok, User_Type intersects)
{
    int k;
    for (int i = 0; i < intersects.types.size(); i++)
    {
        k = 0;
        for (int j = 0; j < spisok.size(); j++)
        {
            if (spisok[j] == intersects.types[i]) k = 1;
        }
        if (k == 0) return false;
    }
    return true;
}




bool Is_structure(json Data, string structuring)
{
    vector <json> wrong1;
    vector <json> wrong2;
    vector <json> holes1;
    vector <json> holes2;
    json Doc{ json::parse(structuring) };
    vector <json> Isit1;
    vector <json> Isit2;
    for (auto& Item : Doc.items())
    {
        json b = Item.value();
        std::string JSON{ b.dump() };
        if (JSON[0] == '[')
        {
            Isit2.push_back(Item.value());
            Isit1.push_back(Item.key());
        }
    }
    for (int i = 0; i < Isit2.size(); i++)
    {
        if (Isit2[i][1] == "Hole")
        {
            holes2.push_back(Isit2[i]);
            holes1.push_back(Isit1[i]);
        }
    }
    for (auto& Item1 : Data.items())
    {
        int k = 0;
        for (auto& Item2 : Doc.items())
        {
            if ((Item1.key() == Item2.key()) && (Item1.value() == Item2.value()))  k = 1;
        }
        if (k == 0)
        {
            wrong1.push_back(Item1.key());
            wrong2.push_back(Item1.value());
        }
    }
    if (wrong2.size() == holes2.size())
    {
        vector <int> check;
        for (int i = 0; i < wrong2.size(); i++)
        {
            string thg;
            if (wrong2[i].is_number_integer() == true) thg = "integer";
            if (wrong2[i].is_number_float() == true) thg = "real";
            if (wrong2[i].is_null() == true) thg = "null";
            if (wrong2[i].is_string() == true) thg = "string";
            if (wrong2[i].is_object() == true) thg = "object";
            for (int j = 0; j < holes2.size(); j++)
            {
                int gf = 0;
                for (int t = 0; t < check.size(); t++)
                {
                    if (j == check[t])
                    {
                        gf = 1;
                        break;
                    }
                }
                if ((holes2[j][2] == thg) && (gf == 0))
                {
                    check.push_back(j);
                    break;
                }
            }
        }
        if (check.size() == wrong2.size())
        {
            for (int i = 0; i < check.size(); i++)
            {
                vector <int> same;
                same.push_back(i);
                for (int j = 0; j < check.size(); j++)
                {
                    if (j != i)
                    {
                        int yt = check[i];
                        int fhd = check[j];
                        if (holes2[yt][0] == holes2[fhd][0]) same.push_back(j);
                    }
                }
                for (int r = 0; r < same.size() - 1; r++)
                {
                    int x1 = same[r];
                    int x2 = same[r + 1];
                    if ((wrong2[x1] != wrong2[x2]) || (wrong1[x1] != wrong1[x2]))  return false;
                }
            }
        }
        else return false;
    }
    else  return false;
    return true;
}


string Is_unific(json Doc1, json Doc2)
{
    if (Doc1.size() != Doc2.size()) return "";
    vector <vector<string>> holes;
    string copy{ R"({)" };
    vector <json> problem1;
    vector <json> problem2;
    vector <json> problem3;
    vector <json> problem4;
    vector <json> keys;
    vector <json> values;
    vector <string> hole;
    for (auto& Item1 : Doc1.items())
    {
        int k = 0;
        for (auto& Item2 : Doc2.items())
        {
            if ((Item1.key() == Item2.key()) && (Item1.value() == Item2.value()))
            {
                k = 1;
                values.push_back(Item1.value());
                keys.push_back(Item1.key());
            }
        }
        if (k == 0) {
            problem1.push_back(Item1.value());
            problem3.push_back(Item1.key());
        }
    }
    for (auto& Item2 : Doc2.items()) {
        int k = 0;
        for (auto& Item1 : Doc1.items())
        {
            if ((Item2.key() == Item1.key()) && (Item2.value() == Item1.value())) k = 1;
        }
        if (k == 0) {
            problem2.push_back(Item2.value());
            problem4.push_back(Item2.key());
        }
    }

    int jkh = 0;
    for (int i = 0; i < problem1.size(); i++)
    {
        std::string JSON{ problem1[i].dump() };
        if (JSON[0] != '[')
        {
            string thg;
            if (problem1[i].is_number_integer() == true) thg = "integer";
            if (problem1[i].is_number_float() == true) thg = "real";
            if (problem1[i].is_null() == true) thg = "null";
            if (problem1[i].is_string() == true) thg = "string";
            if (problem1[i].is_object() == true) thg = "object";
            for (int j = 0; j < problem2.size(); j++)
            {
                int lk = 0;
                int jd = 0;
                std::string JSON2{ problem2[j].dump() };
                if ((JSON2[0] == '[') && (problem2[j][1] == "Hole"))
                {
                    if (problem2[j][2] == thg)
                    {
                        for (int u = 0; u < holes.size(); u++)
                        {
                            if ((problem2[j] == holes[u][0]) && (JSON != holes[u][1])) lk = 1;
                        }
                        if (lk == 0)
                        {
                            for (int u = 0; u < holes.size(); u++)
                            {
                                if (JSON2 == holes[u][0]) jd = 1;
                            }
                            if (jd == 0)
                            {
                                hole.clear();
                                hole.push_back(JSON2);
                                hole.push_back(JSON);
                                holes.push_back(hole);
                                values.push_back(problem1[i]);
                                keys.push_back(problem3[i + jkh]);
                                problem1.erase(problem1.begin() + i);
                                problem2.erase(problem2.begin() + j);
                                i--;
                                jkh++;
                                break;
                            }
                        }
                    }

                }

            }
        }

        if ((JSON[0] == '[') && (problem1[i][1] == "Hole"))
        {
            for (int j = 0; j < problem2.size(); j++)
            {
                int lk = 0;
                int jd = 0;
                std::string JSON2{ problem2[j].dump() };
                if (JSON2[0] != '[')
                {
                    string thg;
                    if (problem2[j].is_number_integer() == true) thg = "integer";
                    if (problem2[j].is_number_float() == true) thg = "real";
                    if (problem2[j].is_null() == true) thg = "null";
                    if (problem2[j].is_string() == true) thg = "string";
                    if (problem2[i].is_object() == true) thg = "object";
                    if (problem1[i][2] == thg)
                    {
                        for (int u = 0; u < holes.size(); u++)
                        {
                            if ((problem1[j] == holes[u][0]) && (JSON2 != holes[u][1])) lk = 1;
                        }
                        if (lk == 0)
                        {
                            for (int u = 0; u < holes.size(); u++)
                            {
                                if (JSON2 == holes[u][0]) jd = 1;
                            }
                            if (jd == 0)
                            {
                                hole.clear();
                                hole.push_back(JSON);
                                hole.push_back(JSON2);
                                holes.push_back(hole);
                                values.push_back(problem2[j]);
                                keys.push_back(problem4[j + jkh]);
                                problem1.erase(problem1.begin() + i);
                                problem2.erase(problem2.begin() + j);
                                i--;
                                jkh++;
                                break;
                            }
                        }
                    }

                }
                if ((JSON2[0] == '[') && (problem2[j][1] == "Hole"))
                {
                    if (problem1[i][2] == problem2[j][2])
                    {
                        values.push_back(problem2[j]);
                        keys.push_back(problem4[j + jkh]);
                        problem1.erase(problem1.begin() + i);
                        problem2.erase(problem2.begin() + j);
                        i--;
                        jkh++;
                        break;
                    }

                }
            }
        }

    }

    if (problem1.size() == 0)
    {
        for (int i = 0; i < keys.size(); i++)
        {
            std::string finalkey{ keys[i].dump() };
            std::string finalvalue{ values[i].dump() };
            if (i != keys.size() - 1)
            {
                copy.append(finalkey);
                copy.append(":");
                copy.append(finalvalue);
                copy.append(",");
            }
            if (i == keys.size() - 1)
            {
                copy.append(finalkey);
                copy.append(":");
                copy.append(finalvalue);
                copy.append("}");
            }
        }
    }
    else return "";
    return copy;
}



void Is_obj_unific1(vector <string> unitypes)
{

    for (int t = 0; t < unitypes.size() - 1; t++)
    {
        json Doc1{ json::parse(unitypes[t]) };
        json Doc2{ json::parse(unitypes[t + 1]) };
        json Doc3{ json::parse(unitypes[t + 1]) };
        json Doc4{ json::parse(unitypes[t]) };
        vector <json> problem1;
        vector <json> problem2;
        vector <string> Final;
        vector <json> values;
        for (auto& Item1 : Doc1)
        {
            int k = 0;
            for (auto& Item2 : Doc2)
            {
                if (Item1 == Item2)
                {
                    k = 1;
                    values.push_back(Item1);
                }
            }
            if (k == 0)  problem1.push_back(Item1);
        }
        for (auto& Item2 : Doc2)
        {
            int k = 0;
            for (auto& Item1 : Doc1)
            {
                if (Item2 == Item1) k = 1;
            }
            if (k == 0) problem2.push_back(Item2);
        }
        string** solve;
        solve = new string * [problem1.size()];
        for (int i = 0; i < problem1.size(); i++)
        {
            solve[i] = new string[problem2.size()];
            for (int j = 0; j < problem1.size(); j++)
            {
                solve[i][j] = "";
            }
        }
        int** check;
        check = new int* [problem1.size()];
        for (int i = 0; i < problem1.size(); i++)
        {
            check[i] = new int[problem2.size()];
            for (int j = 0; j < problem1.size(); j++)
            {
                check[i][j] = 0;
            }
        }
        vector <int> fcf;
        itog.clear();
        for (int i = 0; i < problem1.size(); i++)
        {
            int err = 0;
            for (int j = 0; j < problem2.size(); j++)
            {
                string dir = Is_unific(problem1[i], problem2[j]);
                if (dir != "")
                {
                    err = 1;
                    check[i][j] = 1;
                    solve[i][j] = dir;
                    if (problem1.size() > 1) Matrix(check, 0, fcf, problem1.size());
                    if (problem1.size() == 1)
                    {
                        vector <int> one;
                        one.push_back(0);
                        itog.push_back(one);
                    }
                }
            }
            if (err == 0) return;
        }
        for (int e = 0; e < itog.size(); e++)
        {
            string copy1{ R"([)" };
            for (int i = 0; i < values.size(); i++)
            {
                std::string finalvalue{ values[i].dump() };
                copy1.append(finalvalue);
                if (i == values.size() - 1)
                {
                    if (problem1.size() == 0)
                    {
                        copy1.append("]");
                    }
                    else copy1.append(",");
                }
                else copy1.append(",");
            }
            for (int j = 0; j < problem1.size(); j++)
            {
                int pe = itog[e][j];
                string solvevalue = solve[j][pe];
                copy1.append(solvevalue);
                if (j == problem1.size() - 1)
                {
                    copy1.append("]");
                }
                else copy1.append(",");
            }
            Final.push_back(copy1);
        }
        for (int i = 0; i < Final.size(); i++)
        {
            unitypes[t + 1] = Final[i];
            vector <string> unitypes1 = unitypes;
            unitypes1.erase(unitypes1.begin() + t);
            Is_obj_unific1(unitypes1);
        }
        vector <json> problem3;
        vector <json> problem4;
        vector <string> Final1;
        vector <json> values1;
        for (auto& Item1 : Doc3)
        {
            int k = 0;
            for (auto& Item2 : Doc4)
            {
                if (Item1 == Item2)
                {
                    k = 1;
                    values1.push_back(Item1);
                }
            }
            if (k == 0)  problem3.push_back(Item1);
        }
        for (auto& Item2 : Doc4)
        {
            int k = 0;
            for (auto& Item1 : Doc3)
            {
                if (Item2 == Item1) k = 1;
            }
            if (k == 0) problem4.push_back(Item2);
        }
        string** solve1;
        solve1 = new string * [problem3.size()];
        for (int i = 0; i < problem3.size(); i++)
        {
            solve1[i] = new string[problem4.size()];
            for (int j = 0; j < problem3.size(); j++)
            {
                solve1[i][j] = "";
            }
        }
        int** check1;
        check1 = new int* [problem3.size()];
        for (int i = 0; i < problem3.size(); i++)
        {
            check1[i] = new int[problem4.size()];
            for (int j = 0; j < problem3.size(); j++)
            {
                check1[i][j] = 0;
            }
        }
        vector <int> fcf1;
        itog.clear();
        for (int i = 0; i < problem3.size(); i++)
        {
            int err = 0;
            for (int j = 0; j < problem4.size(); j++)
            {
                string dir = Is_unific(problem3[i], problem4[j]);
                if (dir != "")
                {
                    err = 1;
                    check1[i][j] = 1;
                    solve1[i][j] = dir;
                    if (problem3.size() > 1) Matrix(check1, 0, fcf1, problem3.size());
                    if (problem3.size() == 1)
                    {
                        vector <int> one;
                        one.push_back(0);
                        itog.push_back(one);
                    }
                }
            }
            if (err == 0) return;
        }
        for (int e = 0; e < itog.size(); e++)
        {
            string copy1{ R"([)" };
            for (int i = 0; i < values1.size(); i++)
            {
                std::string finalvalue{ values1[i].dump() };
                copy1.append(finalvalue);
                if (i == values1.size() - 1)
                {
                    if (problem3.size() == 0)
                    {
                        copy1.append("]");
                    }
                    else copy1.append(",");
                }
                else copy1.append(",");
            }
            for (int j = 0; j < problem3.size(); j++)
            {
                int pe = itog[e][j];
                string solvevalue = solve1[j][pe];
                copy1.append(solvevalue);
                if (j == problem3.size() - 1)
                {
                    copy1.append("]");
                }
                else copy1.append(",");
            }
            Final1.push_back(copy1);
        }
        for (int i = 0; i < Final1.size(); i++)
        {
            unitypes[t + 1] = Final1[i];
            vector <string> unitypes1 = unitypes;
            unitypes1.erase(unitypes1.begin() + t);
            Is_obj_unific1(unitypes1);
        }

    }
    int gh = unitypes.size() - 1;
    Typesuni.push_back(unitypes[gh]);
}

void remove1(vector<string>& v)
{
    auto end = v.end();
    for (auto it = v.begin(); it != end; ++it) {
        end = std::remove(it + 1, end, *it);
    }
    v.erase(end, v.end());
}


int main()
{
    setlocale(LC_ALL, "Russian");
    int fd=1;
    
        int choice;
        string name;
        vector<string> names;// вектор имён созданных типов
        vector<string> spisok;// вектор найденных типов в объекте
        vector<User_Type> us_types; //вектор из имен декларированных типов

        User_Type mass;
        User_Type uni;
        User_Type inter;
        User_Type structure;
        User_Type unification;
        
        std::string Data{ R"()" };
        string Data1; //  
        std::cout << ("Введите проверяемый объект : ") << endl;
        getline(cin, Data);
        Data.insert(0, Data1);
        json Do{ json::parse(Data) };
        //["arr","Array","object"]
        //["stru","Struct",{ "bane" : [3,4],"v" : ["y","Hole","string"]}]
        //["unioo","Union","string","integer"]
        //["inters","Intersection","stru","unioo"]
        //["unifica","Unification", [ { "bane" : [3,4],"vane" : ["y","Hole","string"]} ], [ { "bane" : [3,4],"vane" :"sto"} ]]
        string ti;
        if (Do.is_number_integer() == true) spisok.push_back("integer");
        if (Do.is_number_float() == true) spisok.push_back("real");
        if (Do.is_null() == true) spisok.push_back("null");
        if (Do.is_string() == true) spisok.push_back("string");
        if (Do.is_object() == true) spisok.push_back("object");

        while (ti != "end")
        {
            std::string Tp{ R"()" };
            std::cout << ("Введите тип данных(введите 'end' для прекращения ввода, встроенные типы вводить с маленькой буквы) : ") << endl;
            ti = "";
            getline(cin, ti);
            if (ti == "end") break;
            Tp.insert(0, ti);
            json Doe{ json::parse(Tp) };
            std::string Tp1{ Doe.dump() };
            json Doec{ json::parse(Tp1) };

            if (std::string{ Doec[1] } == "Array")
            {

                string massive_type;
                massive_type = std::string{ Doec[2] };
                mass.elem_type = massive_type;
                mass.naming = std::string{ Doec[0] };
                mass.construction_type = std::string{ Doec[1] };
                us_types.push_back(mass);
            }
            if (std::string{ Doec[1] } == "Struct")
            {

                structure.param = std::string{ Doec[2].dump() };
                structure.naming = std::string{ Doec[0] };
                structure.construction_type = std::string{ Doec[1] };
                us_types.push_back(structure);
            }
            if (std::string{ Doec[1] } == "Union")
            {
                uni.naming = std::string{ Doec[0] };
                uni.construction_type = std::string{ Doec[1] };
                for (int j = 2; j < Doec.size(); j++)
                {
                    string uni_type = Doec[j];
                    uni.types.push_back(uni_type);
                }
                us_types.push_back(uni);
            }
            if (std::string{ Doec[1] } == "Intersection")
            {
                inter.naming = std::string{ Doec[0] };
                inter.construction_type = std::string{ Doec[1] };
                for (int j = 2; j < Doec.size(); j++)
                {
                    string inter_type = Doec[j];
                    inter.types.push_back(inter_type);
                }
                us_types.push_back(inter);
            }
            if (std::string{ Doec[1] } == "Unification")
            {
                int ka = 0;
                int kb = 0;
                string unification_type;
                string primtype;


                for (int j = 2; j < Doec.size(); j++)
                {
                    unification_type = std::string{ Doec[j].dump() };
                    unification.types.push_back(unification_type);
                }
                Is_obj_unific1(unification.types);
                remove1(Typesuni);
                unification.types = Typesuni;
                Typesuni.clear();
                if (unification.types.size() != 0)
                {
                    unification.naming = std::string{ Doec[0] };
                    unification.construction_type = std::string{ Doec[1] };

                    us_types.push_back(unification);
                }
                if (unification.types.size() == 0)
                {
                    std::cout << "Унификация невозможна" << endl;
                }
            }
        }
        std::cout << "Имя введённого типа:" << "   " << "Введённый тип:" << endl;
        for (int i = 0; i < us_types.size(); i++)
        {
            std::cout << us_types[i].naming << "                    " << us_types[i].construction_type << endl;
        }
        for (int i = 0; i < us_types.size(); i++)
        {
            if (us_types[i].construction_type == "Array")
            {
                if (Is_us_array(Data, us_types[i].elem_type, us_types, spisok) == true)
                {
                    spisok.push_back(us_types[i].naming);
                }
            }
            if (us_types[i].construction_type == "Struct")
            {
                if (Is_structure(Do, us_types[i].param) == true) spisok.push_back(us_types[i].naming);
            }
            if (us_types[i].construction_type == "Union")
            {
                if (Is_us_union(spisok, us_types[i]) == true) spisok.push_back(us_types[i].naming);
            }
            if (us_types[i].construction_type == "Intersection")
            {
                if (Is_us_intersection(spisok, us_types[i]) == true) spisok.push_back(us_types[i].naming);
            }
            if (us_types[i].construction_type == "Unification")
            {
                for (int r = 0; r < us_types[i].types.size(); r++)
                {
                    json Drg{ json::parse(us_types[i].types[r]) };

                    for (int t = 0; t < Drg.size(); t++)
                    {
  
                        for (auto& Item : Drg[t])
                        {
                            std::string Doec2{ Item.dump() };

                        if (Is_structure(Do, Doec2) == true) spisok.push_back(us_types[i].naming);
                        }
                    }
                }
            }
        }
        std::cout << endl;
        remove1(spisok);
        std::cout << "Найденные типы:" << endl;
        for (int i = 0; i < spisok.size(); i++)
        {
            std::cout << spisok[i] << endl;
        }
        std::cout <<"Закончить работу программы-0" << endl;
        cin >> fd;
    
    return 0;
}

