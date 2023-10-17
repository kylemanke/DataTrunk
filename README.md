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
    1 
