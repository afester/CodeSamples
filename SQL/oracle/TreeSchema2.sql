-- SQL*Plus settings - restrict column sizes and enlarge line sizes and page sizes
SET LINESIZE 1000
SET PAGESIZE 300
SET NUMWIDTH 18
COLUMN Label FORMAT A30
COLUMN Node FORMAT A30
COLUMN Path FORMAT A50

-- Alternative (Better?) tree schema which uses NULL as the parent key for
-- root nodes and which adds a foreign key constraint 

-- Remove existing schema objects
BEGIN DROP_TABLE_IF_EXISTS('TreeNode'); END;
BEGIN DROP_SEQUENCE_IF_EXISTS('TreeNodePk'); END;

-- Create a sequence for the primary key
CREATE SEQUENCE TreeNodePk
START WITH 1 
NOMAXVALUE
CACHE 100; 

-- Create the table which holds the tree nodes
CREATE TABLE TreeNode (
    Id NUMBER(10) PRIMARY KEY,					-- Creates implicit index on Id
    Parent NUMBER(10),
    Label VARCHAR2(100),
    FOREIGN KEY(Parent) REFERENCES TreeNode(Id)	-- does **not** create an index on Parent!
);
CREATE INDEX TreeParentFK ON TreeNode(Parent);


-- Create a trigger for automatic primary key creation 
CREATE OR REPLACE TRIGGER TreeNodePkTrigger
BEFORE INSERT ON TreeNode
FOR EACH ROW
BEGIN
    SELECT TreeNodePk.NEXTVAL
    INTO   :NEW.ID
    FROM   DUAL;
END TreeNodePkTrigger;
/

-- Use an anonymous PL/SQL block to create sample data
DECLARE
    rootId NUMBER;
    newId NUMBER;

BEGIN 
    INSERT INTO TreeNode VALUES(null, null, 'Root Node') RETURNING Id INTO rootId;

    INSERT INTO TreeNode VALUES(null, rootId, 'Node 1') RETURNING Id INTO newId;
    INSERT INTO TreeNode VALUES(null, newId, 'Node 1.1');
    INSERT INTO TreeNode VALUES(null, newId, 'Node 1.2') RETURNING Id INTO newId;

    INSERT INTO TreeNode VALUES(null, newId, 'Node 1.2.1') RETURNING Id INTO newId;
    INSERT INTO TreeNode VALUES(null, newId, 'Node 1.2.1.1');

    INSERT INTO TreeNode VALUES(null, rootId, 'Node 2') RETURNING Id INTO newId;
    INSERT INTO TreeNode VALUES(null, newId, 'Node 2.1');
--    INSERT INTO TreeNode VALUES(0, newId, 'Node 2.2');
    INSERT INTO TreeNode VALUES(null, newId, 'Node 2.3');
    INSERT INTO TreeNode VALUES(null, newId, 'Node 2.4');

    INSERT INTO TreeNode VALUES(null, rootId, 'Node 3') RETURNING Id INTO newId;

    INSERT INTO TreeNode VALUES(null, rootId, 'Node 4') RETURNING Id INTO newId;
    INSERT INTO TreeNode VALUES(null, newId, 'Node 4.1');

    COMMIT;
END;
/


CREATE OR REPLACE PROCEDURE CreateSubTree(parentId IN NUMBER, 
                                          childCount IN NUMBER, 
                                          levelCount IN NUMBER,
                                          label IN VARCHAR2) IS

    newId NUMBER;
    newLabel VARCHAR2(256);

BEGIN
    FOR cCount IN 1..childCount LOOP
        newLabel := label || '.' || cCount;
        INSERT INTO TreeNode VALUES(0, parentId, newLabel) RETURNING Id INTO newId;
        IF levelCount > 1 THEN
            CreateSubTree(newId, childCount, levelCount - 1, newLabel);
        END IF;
    END LOOP;
END;


-- Creates a tree of a specific size.
--
-- @param childCount The number of children each node has
-- @param depth      The number of levels of the tree
CREATE OR REPLACE PROCEDURE CreateTree(childCount IN NUMBER, depth IN NUMBER) IS
    
    newId NUMBER;
    
BEGIN
    INSERT INTO TreeNode VALUES(0, 0, 'Root') RETURNING Id INTO newId;
    CreateSubTree(newId, childCount, depth, 'Node');
    COMMIT;
END;

BEGIN CreateTree(5, 5); END;

SELECT * FROM TreeNode;


-- Hierarchical query
-- Pseudo column: LEVEL
SELECT LEVEL, Id, LPAD (' ', 3 * (LEVEL - 1)) || Label Node
FROM TreeNode
START WITH Parent IS NULL
CONNECT BY PRIOR Id = Parent;

-- Pseudo column: CONNECT_BY_ISLEAF
SELECT LEVEL, CONNECT_BY_ISLEAF Leaf, Id, LPAD (' ', 3 * (LEVEL - 1)) || Label Node
FROM TreeNode
START WITH Parent IS NULL
CONNECT BY PRIOR Id = Parent;

     LEVEL       LEAF         ID NODE
---------- ---------- ---------- ------------------------
         1          0          1 Root Node
         2          0          2    Node 1
         3          1          3       Node 1.1
         3          0          4       Node 1.2
         4          0          5          ANode 1.2.1
         5          1          6             Node 1.2.1.1
         2          0          7    Node 2
         3          1          8       Node 2.1
         3          1          9       Node 2.3
         3          1         10       Node 2.4
         3          1         15       Node 2.2
         2          1         11    Node 3
         2          0         12    Node 4
         3          1         13       Node 4.1
         2          0         19    Node 0
         3          1         20       Node 0.1
         3          1         21       Node 0.2


-- Adding SYS_CONNECT_BY_PATH function - returns the path to the current node (!!)
SELECT LEVEL, SYS_CONNECT_BY_PATH(Id, '/') Path, 
	   Id,    LPAD (' ', 3 * (LEVEL - 1)) || Label Node
FROM TreeNode
START WITH  Id=1
CONNECT BY PRIOR Id = Parent;
     LEVEL SUBSTR(SYS_CONNECT_BY_PATH(ID,'/'),0,60)         ID NODE
---------- ---------------------------------------- ---------- ------------------------
         1 /1                                                1 Root Node
         2 /1/2                                              2    Node 1
         3 /1/2/3                                            3       Node 1.1
         3 /1/2/4                                            4       Node 1.2
         4 /1/2/4/5                                          5          ANode 1.2.1
         5 /1/2/4/5/6                                        6             Node 1.2.1.1
         2 /1/7                                              7    Node 2
         3 /1/7/8                                            8       Node 2.1
         3 /1/7/9                                            9       Node 2.3
         3 /1/7/10                                          10       Node 2.4
         3 /1/7/15                                          15       Node 2.2
         2 /1/11                                            11    Node 3
         2 /1/12                                            12    Node 4
         3 /1/12/13                                         13       Node 4.1
         2 /1/19                                            19    Node 0
         3 /1/19/20                                         20       Node 0.1
         3 /1/19/21                                         21       Node 0.2


SELECT LEVEL, SYS_CONNECT_BY_PATH(Id, '/') Path, 
	   PRIOR Id, Id, LPAD (' ', 3 * (LEVEL - 1)) || Label Node
FROM TreeNode
START WITH  Id=1
CONNECT BY PRIOR Id = Parent;

   LEVEL SUBSTR(SYS_CONNECT_BY_PATH(ID,'/'),0,30)   PRIORID    ID NODE
-------- ----------------------------------------- -------- ----- ---------------------------
       1 /1                                                     1 Root Node
       2 /1/2                                             1     2    Node 1
       3 /1/2/3                                           2     3       Node 1.1
       3 /1/2/4                                           2     4       Node 1.2
       4 /1/2/4/5                                         4     5          ANode 1.2.1
       5 /1/2/4/5/6                                       5     6             Node 1.2.1.1
       2 /1/7                                             1     7    Node 2
       3 /1/7/8                                           7     8       Node 2.1
       3 /1/7/9                                           7     9       Node 2.3
       3 /1/7/10                                          7    10       Node 2.4
       3 /1/7/15                                          7    15       Node 2.2
       2 /1/11                                            1    11    Node 3
       2 /1/12                                            1    12    Node 4
       3 /1/12/13                                        12    13       Node 4.1
       2 /1/19                                            1    19    Node 0
       3 /1/19/20                                        19    20       Node 0.1
       3 /1/19/21                                        19    21       Node 0.2
         

DECLARE
    rootId NUMBER;
    newId NUMBER;

BEGIN 
    INSERT INTO TreeNode VALUES(0, 7, 'Node 2.2');
	INSERT INTO TreeNode VALUES(0, 1, 'Node 0') RETURNING Id INTO newId;
	INSERT INTO TreeNode VALUES(0, newId, 'Node 0.1');
	INSERT INTO TreeNode VALUES(0, newId, 'Node 0.2');
END;
/

-- Note: using the same query as above now correctly returns "Node 2.2" as a sub node of
-- "Node 2", BUT it lists it at the end of all sub nodes of "Node 2"!
-- We can NOT simply use "ORDER BY" since the ORDER BY affects the complete result set:
-- http://docs.oracle.com/database/121/SQLRF/queries003.htm#SQLRF52335
-- "In a hierarchical query, do not specify either ORDER BY or GROUP BY, as they will 
--  override the hierarchical order of the CONNECT BY results. If you want to order 
--  rows of siblings of the same parent, then use the ORDER SIBLINGS BY clause."

SELECT LEVEL, Id, LPAD (' ', 3 * (LEVEL - 1)) || Label Node
FROM TreeNode
START WITH Parent IS NULL
CONNECT BY PRIOR Id = Parent
ORDER BY Label;

     LEVEL         ID NODE
---------- ---------- ------------------------------------------------------------------------------------------------------------------------
         4          5          ANode 1.2.1
         2         19    Node 0
         3         20       Node 0.1
         3         21       Node 0.2
         2          2    Node 1
         3          3       Node 1.1
         3          4       Node 1.2
         5          6             Node 1.2.1.1
         2          7    Node 2
         3          8       Node 2.1
         3         15       Node 2.2
         3          9       Node 2.3
         3         10       Node 2.4
         2         11    Node 3
         2         12    Node 4
         3         13       Node 4.1
         1          1 Root Node

         
-- See how the Root Node and "ANode 1.2.1" (which we have renamed from Node 1.2.1 before)
-- are located at the wrong positions! 

SELECT Parent, ROWNUM, LEVEL, Id, substr( LPAD (' ', 3 * (LEVEL - 1)) || Label, 0, 30) Node
FROM TreeNode
START WITH Parent IS NULL
CONNECT BY PRIOR Id = Parent
ORDER SIBLINGS BY Label;

    ROWNUM      LEVEL         ID NODE
---------- ---------- ---------- ------------------------------------------------------------------------------------------------------------------------
         1          1          1 Root Node
         2          2         19    Node 0
         3          3         20       Node 0.1
         4          3         21       Node 0.2
         5          2          2    Node 1
         6          3          3       Node 1.1
         7          3          4       Node 1.2
         8          4          5          ANode 1.2.1
         9          5          6             Node 1.2.1.1
        10          2          7    Node 2
        11          3          8       Node 2.1
        12          3         15       Node 2.2
        13          3          9       Node 2.3
        14          3         10       Node 2.4
        15          2         11    Node 3
        16          2         12    Node 4
        17          3         13       Node 4.1


       

-- Path query - start with leaf and get all rows up to the root node
SELECT LEVEL, Id,  substr( LPAD (' ', 3 * (LEVEL - 1)) || Label, 0, 30) Node
FROM TreeNode
START WITH  Id=6
CONNECT BY Id = PRIOR Parent;

SELECT LEVEL, substr( SYS_CONNECT_BY_PATH(Label, '/'), 0, 60), 
	   Id,    substr( LPAD (' ', 3 * (LEVEL - 1)) || Label, 0, 30) Node
FROM TreeNode
START WITH  Id=6
CONNECT BY Id = PRIOR Parent;



-- Subquery Factoring Clause

-- General Syntax:
-- WITH query_name ([c_alias [, c_alias]...]) AS (subquery) [search__clause] [cycle_clause]
--   [, query_name ([c_alias [, c_alias]...]) AS (subquery) [search_clause] [cycle_clause]]
-- SELECT ... 

-- Simple example - not recursive! (and not really useful - need to look for a better sample)
--  Query Name Column Aliases
--   vvvvvvvv  vvvvvvv
WITH AllNodes (a, b, c) AS
(SELECT * FROM TreeNode)
 SELECT * FROM AllNodes a, AllNodes b;

-- If a subquery_factoring_clause refers to its own query_name in the subquery 
-- that defines it, then the subquery_factoring_clause is said to be recursive.

-- Incomplete example
WITH AllNodes (a, b, c) AS
(SELECT * FROM AllNodes) -- Refers to its own query name!
 SELECT * FROM AllNodes;

-- A recursive subquery_factoring_clause must contain two query blocks:
--    * The first is the anchor member 
--    * The second is the recursive member
-- The anchor member must appear before the recursive member, and it cannot reference query_name. 
-- The recursive member must follow the anchor member and must reference query_name exactly once. 
-- You must combine the recursive member with the anchor member using the UNION ALL set operator.

-- Simple query which traverses all nodes (bread first - all nodes on the same 
-- level are returned before continuing on the next level)
WITH Node (Id, Parent, Label) AS
(   -- Anchor Member - start node
    SELECT *
    FROM TreeNode
    WHERE Parent IS NULL
  UNION ALL
    -- Recursive Member - join between parent and child
    SELECT ChildNode.Id, ChildNode.Parent, ChildNode.Label
    FROM TreeNode ChildNode
    JOIN Node ParentNode ON (ParentNode.Id = ChildNode.Parent)
)
SELECT * 
FROM Node;

-- Add a level indicator (do not use "Level" as column name since "Level" is a reserved identifier)
WITH Node (Id, Parent, HLevel, Label) AS
(   -- Anchor Member - start node
    SELECT TreeNode.Id, TreeNode.Parent, 1, TreeNode.Label
    FROM TreeNode
    WHERE Parent IS NULL
  UNION ALL
    -- Recursive Member - join between parent and child
    SELECT ChildNode.Id, ChildNode.Parent, ParentNode.HLevel+1, ChildNode.Label
    FROM TreeNode ChildNode
    JOIN Node ParentNode ON (ParentNode.Id = ChildNode.Parent)
)
SELECT * 
FROM Node;

-- Add a Node order sort criteria.  
-- SEARCH mode    BY siblingsOrder        SET orderColumn
--        ^^^^       ^^^^^^^^^^^^^            ^^^^^^^^^^^
--  BREADTH FIRST    Clause to specify        Column name to add
-- or DEPTH FIRST    Ordering of siblings     for the final node sequence
--
-- Default is BREADTH FIRST, so the following query returns the same result as
-- the one above (but adds the NodeSequence column):
WITH Node (Id, Parent, HLevel, Label) AS
(   -- Anchor Member - start node
    SELECT TreeNode.Id, TreeNode.Parent, 1, TreeNode.Label
    FROM TreeNode
    WHERE Parent IS NULL
  UNION ALL
    -- Recursive Member - join between parent and child
    SELECT ChildNode.Id, ChildNode.Parent, ParentNode.HLevel+1, ChildNode.Label
    FROM TreeNode ChildNode
    JOIN Node ParentNode ON (ParentNode.Id = ChildNode.Parent)
)
SEARCH BREADTH FIRST BY Label SET NodeSequence
SELECT * 
FROM Node
ORDER BY NodeSequence;


-- Use DEPTH FIRST search to return a tree structure, and add indentation 
-- of the label according to the level
WITH Node (Id, Parent, HLevel, Label) AS
(   -- Anchor Member - start node
    SELECT TreeNode.Id, TreeNode.Parent, 1, TreeNode.Label
    FROM TreeNode
    WHERE Parent IS NULL
  UNION ALL
    -- Recursive Member - join between parent and child
    SELECT ChildNode.Id, ChildNode.Parent, ParentNode.HLevel+1, ChildNode.Label
    FROM TreeNode ChildNode
    JOIN Node ParentNode ON (ParentNode.Id = ChildNode.Parent)
)
SEARCH DEPTH FIRST BY Label SET NodeSequence
SELECT Id, Parent, HLevel, LPAD (' ', 2 * (HLevel - 1)) || Label AS Label, NodeSequence
FROM Node
ORDER BY NodeSequence;


-- Cycle clause 
