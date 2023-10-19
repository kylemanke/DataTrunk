# DataTrunk
Lightweight Object Database

## Goal
Goal is to implement a lightweight object oriented database that will function similarly to SQLite. It will use one main database file, each database will be split into connections. Each connection will store objects of a certain base type. This base type can then be inherited and serves similar to polymorphism. Each inheritance level can have a primary key or secondary key but the base primary key will serve as the overall primary key. Inherited primary keys will function more similarly to secondary keys. Each inheritance object will have its own B-Tree. Each secondary key will have a B-Tree. Queries based on non-indexed values will be linear in time. The database will not support multiple access at the beginning. Only one connection will be allowed.

## Plan:
    1. Design objects
    2. Design query language
    3. Design Storage format
    4. Implement

### Objects
    1 Will be similar to C++ structs
    2 Data Types (to start):
        - byte -> 8 bit piece of bytes
        - short -> 16 bit int
        - int -> 32 bit int
        - long -> 64 bit int
        - float -> 32 bit floating point
        - double -> 64 bit floating point
        - string -> variable length string -> ""
        - struct -> embedded struct
        - array -> vector of any above types, must be of one type, array of bytes acts as a blob
            - cannot be made a key
            - Can be sorted to improve search efficiency
        - pointers -> hold reference to an item or key somewhere else (*) (maybe for this version)
    3 Modifiers
        - const -> can only be assigned at creation
        - signed/unsigned -> for int object types
        - notnull -> item can't be left null
        - primary -> denotes primary key (one allowed per class hierarchy), implies not null, must be unique in database
        - seconday -> denotes secondary key (as many allowed as needed)
        - = -> default value
    4 Inheritance
        - Can inherit base classes
        - Acts like a sub schema of a trunk
    5 Restrictions
        - Max Object Size -> ....
    6 default functions
        - auto_increment(int initial) -> one more than the last, takes in an initial value, default is 0
        - random_guid() -> returns a unique random guid
        - datetime() -> a string of the current datetime
    7 If a primary key is not assigned, a generic autoincremented counter will be used

example:
struct BaseObject {
    primary int uid;
    secondary notnull string name;
    float gpa = 4.0;
    string[] classes;
}

## Language
- Supports basic CRUD, case matters to start
- Create:
    - Create new database
    - Create new inherited object in database
    - Insert new record
    - Create new data member on an object
- Read:
    - fetch a database
    - Read objects by key value
        - primary key will be fastest
    - ObjectName.key will be the format
    - Search for key values or ranges on supported types
    - Can search in lists but this will be slow as a linear search must be done
        - ordered lists will allow for faster searching
- Update:
    - Update any value unless it is const
    - use Object.name = "new value" 
    - Multiple objects can be updated based on selection
    - Can push_back on lists
- Delete:
    - Delete database
    - Delete selected items
    - Delete only inherited items, keep lower sub classes
    - Delete a data member
    - Remove from a list
- Single Line comments: //
- Like C++ whitespace does not matter unless necessary to separate tokens

## Actual language (Object Oriented, noSQL)
0. Define Objects (must be declared before use) // All objects will have a included key that mark their type
struct embedded_struct {
    ...;
}

struct structName {
    ...;
    string name = "";
    embedded_struct key_;
};

struct SubStruct : SuperStruct {
    string name = "sub_struct"; // overwrites the default value
};

// Will support standard functions: for, while, if, else if, else, functions
// &&, ||, ==, !=, <, <=, >, >=
// Only standard so far will be print
// Language will support variables of all types
// Things are in memory


1. Create a database:
Trunk trunkName = new Trunk(name, base_struct_type);
Trunk trunkName(name, base_struct_type);

2. Create new inherited object
trunkObject.addObjectType(SubStruct);

3. Insert new record
StructType name = StructTypeConstructor(key=valueConstructor(), key=..., ...);
trunkObjet.insert(name);

4. Create new data key (like a static method)
ObjectStruct.Append(key_name, key_type, default_val);

5. Select a trunk
Trunk myTrunk = OpenTrunk(trunk_name);

6. Fetch objects by key value
ReturnObject[] objects = myTrunk.fetch(expression);
-> fetch will return the most common base class of all things used in expression

7. Update singular object: // Use ObjectTypeName to do static updates to all objects of that type
    object.key = new_value;
    object.list.append(new_item);
    object.list[i].key = new_value;

8. Batch update on lists of objects // Will be changed to support more expressive changes but this for now
    objectList.key = new_value;

9. Delete Database
delete trunkName;

10. Delete selected items
delete item (or itemList);

11. Delete SubClass items
delete trunkName.SubClassType;

12. Delete data member
delete ObjectType.key;

13. Remove from a list
listItem.remove(item)

Note: items can be cast with (type)object; Either upcast or downcast but it will be checked



