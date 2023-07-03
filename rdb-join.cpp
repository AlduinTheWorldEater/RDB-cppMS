#include "rdb.h"
#include <algorithm>
using namespace std;

Relation* Relation::naturaljoin(Relation* R, list<string> joinattrs) { //Carries out the NATURAL JOIN operation.

    Relation *R_ = new Relation(*R); //Copy the relation so that no permanent changes occur.
    map<string, string> renamedjoinattrs; //A mapping object to store the renamed column names for the join operation. 
    string strtemplate = "_temp0"; //The template for the renamed column names. They always start with an underscore
    
    for (auto it = attrnames.begin(); it != attrnames.end(); it++) {
        
        if (R -> schema[*it] == 0 && R -> attrnames[0] != *it) continue; //Do not do anyting if it is an exclusive name.

        strtemplate[5]++; //Change the renamed string.
        if (find(joinattrs.begin(), joinattrs.end(), *it) != joinattrs.end()) renamedjoinattrs[*it] = strtemplate; 
        //If the column is present as a join attribute, push it into renamedjoinattributes.

        R_ -> rename(*it, strtemplate); //Rename
    }
    
    //Get the Cartesian Product of the renamed table and the table on which it is called.
    Relation* R1 = CartesianProduct(R_);
    
    list<string> projectionlist(attrnames.begin(), attrnames.end()); //The list of column names without the renamed columns.
    int i = 0; //Counter for the renames vector.

    for (auto it = R_ -> attrnames.begin(); it != R_ -> attrnames.end(); it++) {

        if ((*it)[0] == '_')  {

            i++;
            continue;
        }

        projectionlist.push_back(*it); //The unique non-renamed column names are pushed into the projectionlist.
    }

    //Create the DNFformula struct for the selection operation.
    DNFformula form; 
    
    char c = '='; //Always run an equal to operation, so char c is fixed
    Attr* tempattr;
    tuple<string, char, Attr*> tup;
    
    //The DNF for Natural Join looks like: Record 1 OR Record 2 OR Record 3 Or ...
    //By Record 1, I mean the values in the set of columns common in both tables that occur in Record 1. 

    for (auto it = R_ -> recs.begin(); it != R_ -> recs.end(); it++) { //Run a selection for each record in R_.
        
        list<tuple<string, char, Attr*>> lis;

        for (auto it1 = joinattrs.begin(); it1 != joinattrs.end(); it1++) { //The conjunction of all attributes per row.

            tempattr = (*it) -> attrptr[R_ -> schema[renamedjoinattrs[*it1]]]; //Get the attribute required.

            tup = make_tuple(*it1, c, tempattr); //Make the tuple tup.
            lis.push_back(tup); 

            tup = make_tuple(renamedjoinattrs[*it1], c, tempattr); //We need the other attribute too, to compare.
            lis.push_back(tup); //lis now stores the normal form of one row.
        }

        form.ops.push_back(lis); //Creating the DNF formula.
    }

    R1 = R1 -> selection(form); //Select needed rows from the cartesian Product

    R1 = R1 -> projection(projectionlist); //Take the projection.

    return R1;
}
