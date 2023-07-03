#include "rdb.h"
using namespace std;

void printmap(map<string, Relation*> obj) {

    cout << "The following are the names of the relations stored in the program: " << endl << endl;
    for (auto it = obj.begin(); it != obj.end(); it++) {

        cout << "Name: " << it -> first << endl;
    }

    cout << endl;
} 

void menu() {

    cout << "Welcome to the Reational Database model!" << endl
         << "Please enter the numeric codes corresponding to the relevant functions, as given below" << endl << endl
         << "1. Enter 1 to create, but not populate, a relation." << endl
         << "2. Enter 2 to populate, but not create, an existing relation." << endl
         << "3. Enter 12 to create and populate a relation." << endl
         << "4. Enter 3 to delete a relation." << endl
         << "5. Enter 4 to add a record to an existing table." << endl
         << "6. Enter 5 to print the relation in tabular form." << endl
         << "7. Enter 6 to create a table from existing tables using defined operations." << endl
         << "8. Enter 7 to print the menu again." << endl 
         << "9. Enter 8 to exit the program" << endl << endl;
}
int main() {

    map<string, Relation *> tablemap; //A mapping object storing a tablename and it's associated relation

    int menuvar;

    menu();

    while (1) {
        
        cout << "Please enter the desired code: ";
        cin >> menuvar;

        if (menuvar == 1) {

            cout << "Enter the name of the table: ";
            
            string name;
            cin >> name;

            Relation* r = NULL;
            tablemap[name] = r;
        }
        
        else if (menuvar == 2) {

            printmap(tablemap);
            cout << "Enter the name of the table to populate: ";
            
            string name;
            cin >> name;

            cout << "Enter the number of attributes and rows you want to generate (space-separated ints): ";
            
            int attr, rec;
            cin >> attr >> rec;

            Relation* r = new Relation(attr, rec);
            
            tablemap[name] = r;

            r -> generateRelation();
        }
        
        else if (menuvar == 12) {

            cout << "Enter the name of the table: ";
            
            string name;
            cin >> name;

            cout << "Enter the number of attributes and rows you want to generate (space-separated ints): ";
            
            int attr, rec;
            cin >> attr >> rec;

            Relation* r = new Relation(attr, rec);
            r -> generateRelation();

            tablemap[name] = r;
        }

        else if (menuvar == 3) {

            printmap(tablemap);
            cout << "Enter the name of the table to delete: ";
            
            string name;
            cin >> name;

            Relation* r = tablemap[name];
            tablemap.erase(name);

            delete r;
        }
        
        else if (menuvar == 4) {

            printmap(tablemap);
            cout << "Enter the name of the table to add to: ";
            
            string name;
            cin >> name;

            Relation* r = tablemap[name];

            r -> addRecord(1);
        }
        
        else if (menuvar == 5) {

            printmap(tablemap);
            cout << "Enter the name of the table to print: ";
            
            string name;
            cin >> name;

            if (tablemap[name]) tablemap[name] -> print();
        }
        
        else if (menuvar == 6) {
            
            cout << "Enter the code of the operation you wish to perform." << endl << endl
                 << "The list of available operations are: " << endl
                 << "1. Union" << endl
                 << "2. Difference" << endl
                 << "3. Cartesian Product" << endl
                 << "4. Projection" << endl
                 << "5. Selection" << endl
                 << "6. Rename" << endl 
                 << "7. Natural Join" << endl << endl;

            int code;
            cout << "Enter the code: ";
            cin >> code;

            if (code == 1) {

                printmap(tablemap);
                string name1, name2, name;

                cout << "Enter the names of the two tables to be joined (space-separated format): ";
                cin >> name1 >> name2;

                cout << "Enter the name of the new table: ";
                cin >> name;
                cout << endl;

                Relation *r1 = tablemap[name1], *r2 = tablemap[name2];
                Relation *r3 = r1 -> Union(r2);
                tablemap[name] = r3;
                cout << endl << "Printing " << name << ":" << endl;
                r3 -> print();
            }

            else if (code == 2) {

                printmap(tablemap);
                string name1, name2, name;

                cout << "Enter the names of the two tables to be joined (space-separated format, in order for difference): ";
                cin >> name1 >> name2;
                cout << "Enter the name of the new table: ";
                cin >> name;
                cout << endl;

                Relation *r1 = tablemap[name1], *r2 = tablemap[name2];
                Relation *r3 = r1 -> difference(r2);

                tablemap[name] = r3;
                cout << endl << "Printing " << name << ":" << endl;
                r3 -> print();
            }

            else if (code == 3) {

                printmap(tablemap);
                string name1, name2, name;

                cout << "Enter the names of the two tables to be joined (space-separated format): ";
                cin >> name1 >> name2;
                cout << "Enter the name of the new table: ";
                cin >> name;
                cout << endl;

                Relation *r1 = tablemap[name1], *r2 = tablemap[name2];
                Relation *r3 = r1 -> CartesianProduct(r2);

                tablemap[name] = r3;
                cout << endl << "Printing " << name << ":" << endl;
                r3 -> print();
            }

            else if (code == 4) {

                printmap(tablemap);
                string name1, name;

                cout << "Enter the name of the table to be renamed and stored separately: ";
                cin >> name1;

                cout << "Enter the new name of the table: ";
                cin >> name;
                cout << endl;
                

                Relation *r1 = tablemap[name1];
                
                list<string> lis;
                string attribs;

                while (1) {

                    cout << "Enter the attributes to be projected, -- to exit: ";
                    cin >> attribs;

                    if (attribs == "--") break;

                    lis.push_back(attribs);
                }

                Relation* r2 = r1 -> projection(lis);

                tablemap[name] = r2;
                cout << endl << "Printing " << name << ":" << endl;
                r2 -> print();
            }

            else if (code == 5) {

                printmap(tablemap);
                string name1, name;

                cout << "Enter the name of the table to be renamed and stored separately: ";
                cin >> name1;

                cout << "Enter the new name of the table: ";
                cin >> name;
                cout << endl;

                Relation *r1 = (tablemap[name1]);
                DNFformula f;

                tuple<string, char, Attr*> tup;

                cout << "Valid operations are: " << endl
                         << "1. Equal to - =" << endl
                         << "2. Not equal to - !" << endl
                         << "3. Less than - <" << endl
                         << "4. Greater than - >" << endl
                         << "5. Less thhan or equal to - l" << endl
                         << "6. Greater than or equal to - g" << endl;
            
                while (1) {
                    
                    list<tuple<string, char, Attr*>> lis;
                    while (1) {
                        
                        cout << "Enter the column name and operation to be performed (space-separated format): ";

                        string colname;
                        char c, c1;
                        types comp;

                        cin >> colname >> c;
                        comp = r1 -> dtypes[r1 -> schema[colname]];
                        
                        if (comp == intType) {

                            int i;
                            cout << "Enter the value for comparison: ";
                            cin >> i;

                            integerAttribute* tempattr = new integerAttribute(i);

                            tup = make_tuple(colname, c, tempattr);
                        }

                        else if (comp == floatType) {

                            float i;
                            cout << "Enter the value for comparison: ";
                            cin >> i;

                            floatAttribute* tempattr = new floatAttribute(i);

                            tup = make_tuple(colname, c, tempattr);
                        }

                        else {

                            string i;
                            cout << "Enter the value for comparison: ";
                            cin >> i;

                            stringAttribute* tempattr = new stringAttribute(i);

                            tup = make_tuple(colname, c, tempattr);
                        }

                        lis.push_back(tup);

                        cout << "Do you wish to add an AND condition or not? (y/n): ";
                        cin >> c1;

                        if (c1 == 'n') break;
                    }

                    f.ops.push_back(lis);
                    char c1;

                    cout << "Do you wishh to add an OR condition? (y/n): ";
                    cin >> c1;

                    if (c1 == 'n') break;
                }

                Relation* r2 = r1 -> selection(f);

                tablemap[name] = r2;
                cout << endl << "Printing " << name << ":" << endl;
                r2 -> print();
            }
            
            else if (code == 6) {

                printmap(tablemap);
                string name1, name;

                cout << "Enter the name of the table to be renamed and stored separately: ";
                cin >> name1;

                cout << "Enter the new name of the table: ";
                cin >> name;

                Relation *r1 = new Relation(*(tablemap[name1]));
                string oldname, newname;

                cout << "Enter the original name of the column you wish to rename: ";
                cin >> oldname;

                cout << "Enter the new name of the column: ";
                cin >> newname;

                r1 -> rename(oldname, newname);

                tablemap[name] = r1;
                cout << endl << "Printing " << name << ":" << endl;
                r1 -> print();
            }

            else if (code == 7) {

                printmap(tablemap);
                string name1, name2, name;

                cout << "Enter the names of the two tables to be joined (space-separated format): ";
                cin >> name1 >> name2;

                cout << "Enter the name of the new table: ";
                cin >> name;
                cout << endl;

                Relation *r1 = tablemap[name1], *r2 = tablemap[name2];

                list<string> joinattrs;

                while (1) {

                    cout << "Enter the joining column name (-- to finish): ";
                    string colname;
                    cin >> colname;
                    
                    if (colname == "--") break;

                    joinattrs.push_back(colname);
                }

                Relation *r3 = r1 -> naturaljoin(r2, joinattrs);
                tablemap[name] = r3;
                cout << endl << "Printing " << name << ":" << endl;
                r3 -> print();
            } 
            
            else {

                cout << "Invalid code!" << endl << endl;
            }
        }

        else if (menuvar == 7) {

            menu();
            continue;
        }
        
        else if (menuvar == 8) {

            cout << "Thank you for using the program! Please leave a review with our agents!" << endl;
            break;
        }
        
        else {

            cout << "Unknown menu code entered! Please enter the correct code!" << endl << endl;
            continue;
        }
    }

}