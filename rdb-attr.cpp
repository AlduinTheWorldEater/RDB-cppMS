#include "rdb.h"
#include <algorithm>

using namespace std;

//Overload >> for the en enumeration.

istream& operator >> (istream& in, types& e) { // Operator overloading to help in input of the column type. 

    string s;

    in >> s;

    try{
        
        if (s == "int") e = intType;
        else if (s == "float") e = floatType;
        else if (s == "string") e = stringType;
        
        else throw s;
    }

    catch(string s) {

        cout << "Invalid type entered! " << s << " is not a valid type!" << endl;
        e = ErrorType; //Throws error if any type other than the three mentioned types are entered.
    }

    return in;
}

//Class definitions for integerAttribute

/*

    All == operator overloads check for the equality of the member data.
    All != operator overloads check for the inequality of the member data.
    All > operator overloads check if the member data is greater than the right object.
    All < operator overloads check if the member data is lesser than the right object.
    All >= operator overloads check if the member data is greater than or equal to the right object.
    All <= operator overloads check for if the member data is lesser than or equal to the right object.
*/

integerAttribute::integerAttribute(int value) : val(value) {} //Constructor

bool integerAttribute::operator ==(const Attr& right) const {

    return (val == static_cast<const integerAttribute&>(right).val);
}

bool integerAttribute::operator !=(const Attr& right) const {

    return (val != static_cast<const integerAttribute&>(right).val);
}

bool integerAttribute::operator <(const Attr& right) const {

    return (val < static_cast<const integerAttribute&>(right).val);
}

bool integerAttribute::operator <=(const Attr& right) const {

    return (val <= static_cast<const integerAttribute&>(right).val);
}

bool integerAttribute::operator >(const Attr& right) const {

    return (val > static_cast<const integerAttribute&>(right).val);
}

bool integerAttribute::operator >=(const Attr& right) const {

    return (val >= static_cast<const integerAttribute&>(right).val);
}


//Class definitions for floatAttribute
floatAttribute::floatAttribute(float value) : val(value) {}

bool floatAttribute::operator ==(const Attr& right) const {

    return (val == static_cast<const floatAttribute&>(right).val);
}

bool floatAttribute::operator !=(const Attr& right) const {

    return (val != static_cast<const floatAttribute&>(right).val);
}

bool floatAttribute::operator <(const Attr& right) const {

    return (val < static_cast<const floatAttribute&>(right).val);
}

bool floatAttribute::operator <=(const Attr& right) const {

    return (val <= static_cast<const floatAttribute&>(right).val);
}

bool floatAttribute::operator >(const Attr& right) const {

    return (val > static_cast<const floatAttribute&>(right).val);
}

bool floatAttribute::operator >=(const Attr& right) const {

    return (val >= static_cast<const floatAttribute&>(right).val);
}


//Class definitions for stringAttribute
stringAttribute::stringAttribute(string str) : name(str) {}

bool stringAttribute::operator ==(const Attr& right) const {

    return (name == static_cast<const stringAttribute&>(right).name);
}

bool stringAttribute::operator !=(const Attr& right) const {

    return (name != static_cast<const stringAttribute&>(right).name);
}

bool stringAttribute::operator <(const Attr& right) const {

    return (name < static_cast<const stringAttribute&>(right).name);
}

bool stringAttribute::operator <=(const Attr& right) const {

    return (name <= static_cast<const stringAttribute&>(right).name);
}

bool stringAttribute::operator >(const Attr& right) const {

    return (name > static_cast<const stringAttribute&>(right).name);
}

bool stringAttribute::operator >=(const Attr& right) const {

    return (name >= static_cast<const stringAttribute&>(right).name);
}

//Class definitions for Record Class.

void Record::push(Attr* atptr) { //Pushes into the vector a pointer to an Attribute class instance.

    attrptr.push_back(atptr);
}

void Record::print() { //Prints out the data in a Record. Done through a static cast.

    int counter;

    for (counter = 0; counter < attrptr.size(); counter++) {

        if (typeid(*(attrptr[counter])) == typeid(integerAttribute)) 
            cout << static_cast<integerAttribute *>(attrptr[counter]) -> val << "\t";

        else if (typeid(*(attrptr[counter])) == typeid(floatAttribute)) 
            cout << static_cast<floatAttribute *>(attrptr[counter]) -> val << "\t";

        else if (typeid(*(attrptr[counter])) == typeid(stringAttribute))
            cout << static_cast<stringAttribute *>(attrptr[counter]) -> name << "\t";
    } 

    cout << endl;
}

bool Record::operator ==(const Record& right) const { //Element-wise equality check.

    int i;

    if (attrptr.size() != (right.attrptr.size())) return false;

    for (i = 0; i < attrptr.size(); i++) {

        if (*(attrptr[i]) != *(right.attrptr[i])) return false;
    }

    return true;
}

//Class definitons for class Relation

void Relation::generateHeader() { //Interactively generates the column headers.

    string temp;
    types j;
    int i;

    cout << "Enter, in each new line, the name and data type of columns separated by a whitespace." << endl << endl;

    for (i = 0; i < nattr; i++) {
        
        cout << "Attribute " << i + 1 << ":\t";
        cin >> temp >> j;

        if (j == ErrorType) {

            i--;
            continue;
        }

        attrnames.push_back(temp); //Pushes input data into the respective containers.

        attrinds.push_back(i);
        schema[attrnames[i]] = i;

        dtypes.push_back(j);
    }

    cout << endl;
}

void Relation::generateRecords() { //Interactively generates the records.
    
    int i;

    for (i = 0; i < nrecs; i++) {

        addRecord(0);
    }
}

Relation::Relation(int numattr, int numrecs) : nattr(numattr), nrecs(numrecs) {}; //Two constructors, one for general use and the other for functional use.
Relation::Relation(int numattr) : nattr(numattr) { nrecs = 0;}

Relation::~Relation() { //Destructor.

    for (auto it = recs.begin(); it != recs.end(); it++) {

        delete *it;
    }
};

Relation::Relation(Relation& r): nattr(r.nattr), nrecs(r.nrecs), attrnames(r.attrnames), attrinds(r.attrinds), recs(r.recs), dtypes(r.dtypes), schema(r.schema) {}; //Copy constructor.

void Relation::generateRelation() { //Generates the table. Needs an explicit call.

    generateHeader();
    generateRecords();
}

int Relation::checkredundancy(Record rec) { //Checks if a record is already present in a table.

    for (auto it = recs.begin(); it != recs.end(); it++) {

        if (*(*it) == rec) return 1;
    }

    return 0;
}

void Relation::print() { //Helper utility to print the table out.

    cout << "Printing in the order of:\t";
    
    for (int i = 0; i < nattr - 1; i++) cout << attrnames[i] << "  |  ";
    
    cout << attrnames[nattr - 1] << endl << endl;
    int i = 1;

    for (auto it = recs.begin(); it != recs.end(); it++) {

        cout << "Record " << i << ":\t";
        (*it) -> print();
        cout << endl;
        i++;
    }
}

void Relation::addRecord(int check) { //Adds a record. check toggles if it's a generation or if it's an addition.

    Record *R = new Record();
    int i = 0;

    for (auto it = dtypes.begin(); it != dtypes.end(); it++) {

        types ele = *it; //Get thhe type. Based on type, get inputs.

        if (ele == intType) {

            int element;
            cout << "Enter the INTEGER value of the field " << attrnames[i] << ": ";
            cin >> element;

            integerAttribute *tempattr = new integerAttribute(element);
            R -> push(tempattr);
        }

        else if (ele == floatType) {

            float element;
            cout << "Enter the FLOATING POINT DECIMAL value of the field " << attrnames[i] << ": ";
            cin >> element;

            floatAttribute *tempattr = new floatAttribute(element);
            R -> push(tempattr);
        }

        else {

            string element;
            cout << "Enter the string value of the field " << attrnames[i] << ": ";
            cin >> element;

            stringAttribute *tempattr = new stringAttribute(element);
            R -> push(tempattr);
        }

        i++;
    }

    if ( checkredundancy(*R)) { //If redundant, call again, until an irredundant input is given. 
        
        cout << "Entered record already exists! Duplicate records WILL NOT be stored!" << endl;
        addRecord(check);
    }

    else {
    
        recs.push_back(R);
        cout << endl;

        if (check != 0) nrecs++; //Add to nrecs if addition, do not do so if generation.
    }
}

