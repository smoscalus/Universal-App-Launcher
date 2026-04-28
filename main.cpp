#include <iostream>
#include "install/include/hellnah/Engine/Database.h"

using namespace std;

struct student
{
    char Value[32];
    int age;
};

int main()
{
    auto db = Engine::Database("students.hellnot");

    auto students = db.open_table<student>("students");

    auto id = students.insert({"Alex", 20});
    auto student = students.get(id);

    students.remove(id);

    return 1;
}