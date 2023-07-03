#ifndef RDB_H
#define RDB_H

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <map>
using namespace std;


class Attr {

    public:
    virtual bool operator ==(const Attr& right) const = 0;
    virtual bool operator !=(const Attr& right) const = 0;
    virtual bool operator <(const Attr& right) const = 0;
    virtual bool operator <=(const Attr& right) const = 0;
    virtual bool operator >(const Attr& right) const = 0;
    virtual bool operator >=(const Attr& right) const = 0;
};


class integerAttribute: public Attr {

    int val;

    public:

    integerAttribute(int value);
    ~integerAttribute() {};

    virtual bool operator ==(const Attr& right) const override;
    virtual bool operator !=(const Attr& right) const override;
    virtual bool operator <(const Attr& right) const override;
    virtual bool operator <=(const Attr& right) const override;
    virtual bool operator >(const Attr& right) const override;
    virtual bool operator >=(const Attr& right) const override;

    friend class Record;
    friend class Relation;
};

class floatAttribute: public Attr {

    float val;

    public:

    floatAttribute(float value);
    ~floatAttribute() {};

    virtual bool operator ==(const Attr& right) const override;
    virtual bool operator !=(const Attr& right) const override;
    virtual bool operator <(const Attr& right) const override;
    virtual bool operator <=(const Attr& right) const override;
    virtual bool operator >(const Attr& right) const override;
    virtual bool operator >=(const Attr& right) const override;

    friend class Record;
    friend class Relation;
};

class stringAttribute: public Attr {

    string name;

    public:

    stringAttribute(string str);
    ~stringAttribute() {};

    virtual bool operator ==(const Attr& right) const override;
    virtual bool operator !=(const Attr& right) const override;
    virtual bool operator <(const Attr& right) const override;
    virtual bool operator <=(const Attr& right) const override;
    virtual bool operator >(const Attr& right) const override;
    virtual bool operator >=(const Attr& right) const override;

    friend class Record;
    friend class Relation;
};


enum types {
    intType,
    floatType,
    stringType,
    ErrorType
}; 

istream& operator >> (istream& in, types& e);


typedef struct {

    list<list<tuple<string, char, Attr *>>> ops;

} DNFformula;

class Record {

    vector <Attr *> attrptr;

    public:

    Record() {};
    ~Record() {};

    bool operator ==(const Record& right) const;

    void push(Attr* atptr);

    void print();

    friend class Relation;
};

class Relation { // storing a relation
    
    int nattr, nrecs; // number of attributes and records
    vector <string> attrnames; // schema
    vector <int> attrinds; // mapping schema to indices
    list <Record *> recs; // list of records
    
    void generateHeader(); //Interactively generates the column headers.

    void generateRecords(); //Interactively generates the records.

    
    public:
    Relation(int numattr, int numrecs); //Two constructors, one for general use and the other for functional use.
    Relation(int numattr);
    
    ~Relation(); //Destructor.

    Relation(Relation& r); //Copy constructor.
    vector <types> dtypes; // list of datatypes of each attribute
    map <string, int> schema; //Schema mapping object.

    void generateRelation(); //Generates the table. Needs an explicit call.
    int checkredundancy(Record rec); //Checks if a record is already present in a table.
    void rename(string oldname, string newname); //Renames in-place, changges the name in te column vector as well as the schema.
    void print(); //Helper utility to print the table out.
    void addRecord(int check); //Adds a record. check toggles if it's a generation or if it's an addition.

    Relation* Union(Relation* R); //Implements the UNION operation.
    Relation* difference(Relation* R); //Implements the difference operation.
    Relation* CartesianProduct(Relation* R); //Implements the CARTESIAN PRODUCT operation
    Relation* projection(list<string> projectattrs); //Implements the PROJECTION operation
        //This is an ordered function, i.e, the order of elements in the list affects the result printed.

    Relation* selection(DNFformula f); //Implements the SELECT opration
    Relation* naturaljoin(Relation* R, list<string> joinattrs); //Carries out the NATURAL JOIN operation.
};


#endif