#include "rdb.h"
using namespace std;

void Relation::rename(string oldname, string newname) { //Renames in-place, changges the name in te column vector as well as the schema.

    schema.insert({oldname, -1});
    schema.insert({newname, -1}); //Checks if either name is already present. Insert does not work if a name is already existing in the table.

    if (schema[oldname] == -1) {

        cout << "The given old name does not exist in the relation!" << endl;
        return;
    }

    if (schema[newname] != -1) {

        cout << "The given new name already exists in the relation! Overwrite? (yes/no): " << endl;
        string opt;
        cin >> opt;

        if (opt == "no")
            return;
    }

    attrnames[schema[oldname]] = newname;
    schema[newname] = schema[oldname];
    schema.erase(oldname); //Deletes oldname from the schema.
}

Relation* Relation::Union(Relation* R) { //Implements the UNION operation.

    if (nattr != R -> nattr) {

        cout << "Number and type of attributes need to be the same for a union!" << endl;
        return NULL; 
    }

    int i;
    
    for (i = 0; i < nattr; i++) {

        if (dtypes[i] != R -> dtypes[i]) {

            cout << "Number and type of attributes need to be the same for a union!" << endl;
            return NULL;
        }
    }

    //Returns NULL if number and/or data types of columns do not match.

    Relation* R1 = new Relation(*this); //Generate a copy of the object table. Union is simply addition of the rows of the other table.

    for (auto it = (R -> recs).begin(); it != (R -> recs).end(); it++) {

        if ( R1 -> checkredundancy(*(*it))) continue; //If reduundant, do not add.

        (R1 -> recs).push_back(*it);
        (R1 -> nrecs)++;
    } 

    //Add the rows of R to the rows of R1 to generate union.
    
    return R1;
}

Relation* Relation::difference(Relation* R) { //Implements the difference operation.

    if (nattr != R -> nattr) {

        cout << "Number and type of attributes need to be the same for difference!" << endl;
        return NULL;
    }

    int i;
    
    for (i = 0; i < nattr; i++) {

        if (dtypes[i] != R -> dtypes[i]) {

            cout << "Number and type of attributes need to be the same for difference!" << endl;
            return NULL;
        }
    }

    //Return NULL if number and/or types of columns do not match.
    
    Relation* R1 = new Relation(nattr);

    R1 -> attrnames = attrnames;
    R1 -> attrinds = attrinds;
    R1 -> dtypes = dtypes;
    R1 -> schema = schema; //Do not copy the list of records into R1, do it selectively.

    for (auto it = recs.begin(); it != recs.end(); it++) {

        if ( R -> checkredundancy( *(*it))) continue; //If the record *it is present in R, do not push into R1.

        (R1 -> recs).push_back(*it);
        R1 -> nrecs++;
    } // Finishes pushing each unique record into R1.

    return R1;
}

Relation* Relation::CartesianProduct(Relation* R) { //Implements the CARTESIAN PRODUCT operation
    
    Relation *R1 = new Relation(nattr + R -> nattr, nrecs * (R -> nrecs)); //The number of attributes is the sum of individual nattr, 
    //The number of records is the product of each nrecs.

    R1 -> attrnames = attrnames;
    (R1 -> attrnames).insert((R1 -> attrnames).end(), (R -> attrnames).begin(), (R -> attrnames).end());
    //Adds the column names of R to R1, which has column names of the object.

    R1 -> dtypes = dtypes;
    (R1 -> dtypes).insert((R1 -> dtypes).end(), (R -> dtypes).begin(), (R -> dtypes).end());
    //Adds dtypes of R into R1, which has dtype list of the object.

    int i;
    for (i = 0; i < R1 -> nattr; i++) {

        (R1 -> attrinds).push_back(i);
        R1 -> schema[(R1 -> attrnames)[i]] = i;
        //Push the schema and attrinds valuees. 
    }

    //Constructed the header parameters.

    for (auto it1 = recs.begin(); it1 != recs.end(); it1++) {

        for (auto it2 = (R -> recs).begin(); it2 != (R -> recs).end(); it2++) {

            Record* temp = new Record();
            for (i = 0; i < nattr; i++) temp -> push((*it1) -> attrptr[i]);

            for (i = 0; i < R -> nattr; i++) temp -> push((*it2) -> attrptr[i]);

            //Push the values of individual records into a new record object, and store it in the relation.
            (R1 -> recs).push_back(temp);
        }            
    }

    return R1;
}

Relation* Relation::projection(list<string> projectattrs) { //Implements the PROJECTION operation
    //This is an ordered function, i.e, the order of elements in the list affects the result printed.

    Relation* R1 = new Relation(projectattrs.size(), nrecs);
    int i = 0;
    vector<int> projectindices; //The indices where the columns to be projected are stored.

    for (auto it = projectattrs.begin(); it != projectattrs.end(); it++) {
        
        if (schema[*it] == 0) {

            if (*it != attrnames[0]) {

                cout << "Given column does not exist in the relation!" << endl;
                return NULL; 
            }
        } //Return NULL if invalid column is present.

        R1 -> attrnames.push_back(*it);
        R1 -> attrinds.push_back(i);
        R1 -> dtypes.push_back(dtypes[schema[*it]]);
        R1 -> schema[*it] = i;
        projectindices.push_back(schema[*it]); //Stores the indices of headers in order of how it was given.
        //Construct the relation headers otherwise, element by element
        i++;
    }

    for (auto it = recs.begin(); it != recs.end(); it++) { //The addition of records.

        Record* temp = new Record(); //New record object to store the constructed record.
        for (auto it1 = projectindices.begin(); it1 != projectindices.end(); it1++) {

            temp -> push( (*it) -> attrptr[*it1]); //Only push those elements that are in projectattrs
        }

        if (R1 -> checkredundancy(*temp)) {

            delete temp;   
            continue;
        } //If redundant, continue ahead
        
        R1 -> recs.push_back(temp); //If not, push into R1
    }

    return R1;
}

Relation* Relation::selection(DNFformula f) { //Implements the SELECT opration
    
    Relation* R = new Relation(nattr);
    
    R -> attrnames = attrnames;
    R -> attrinds = attrinds;
    R -> dtypes = dtypes;
    R -> schema = schema; //Construct the headers.
    
    string str;
    char c;
    Attr* tempattr; //Variables for convenience.

    for (auto it = recs.begin(); it != recs.end(); it++) {

        Record* temp = *it; //Convenience variable.
        int added = 0;

        for (auto it1 = f.ops.begin(); it1 != f.ops.end(); it1++) {
            //Holds the disjunctive pairs. Each new list *it1 is a list of conditions tobe met simultaneously
            //and if any one *it1 is met then the record is selected.
            //If any one of these condtions are met, add to new relation.
            
            bool satisfy = true;

            for (auto it2 = (*it1).begin(); it2 != (*it1).end(); it2++) {

                tie(str, c, tempattr) = *it2; //Convenience relation. Separate out the tuple.
                
                //Implement for each of the 6 operations defined.
                if (c == '=') {

                    if (*(temp -> attrptr[schema[str]]) == *tempattr) continue;

                    else {

                        satisfy = false;
                        break;
                    }
                }

                else if (c == '<') {

                    if (*(temp -> attrptr[schema[str]]) < *tempattr) continue;

                    else {

                        satisfy = false;
                        break;
                    }
                }

                else if (c == '>') {

                    if (*(temp -> attrptr[schema[str]]) > *tempattr) continue;

                    else {

                        satisfy = false;
                        break;
                    }
                }

                else if (c == 'g') {

                    if (*(temp -> attrptr[schema[str]]) >= *tempattr) continue;

                    else {

                        satisfy = false;
                        break;
                    }
                }

                else if (c == 'l') {

                    if (*(temp -> attrptr[schema[str]]) <= *tempattr) continue;

                    else {

                        satisfy = false;
                        break;
                    }
                }

                else if (c == '!') {

                    if (*(temp -> attrptr[schema[str]]) != *tempattr) continue;

                    else {

                        satisfy = false;
                        break;
                    }
                }
                
                else { //If an undefined operation is used, give Error.

                    cout << "Given operation is not defined!" << endl;
                    return NULL;
                }
            }

            if (satisfy && added == 0) { //If not added previously and if the conditions are satisfied, add to new table.
                
                R -> recs.push_back(*it);
                (R -> nrecs)++;
                added = 1;
            } 
        }
    }

    //Return the new table. 

    return R;
}