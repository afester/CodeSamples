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
    Id NUMBER(10),
    Parent NUMBER(10),
    Label VARCHAR2(100)
);
CREATE UNIQUE INDEX TreePK ON TreeNode(Id);
CREATE INDEX TreeParentPK ON TreeNode(Parent);


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
    INSERT INTO TreeNode VALUES(0, 0, 'Root Node') RETURNING Id INTO rootId;

    INSERT INTO TreeNode VALUES(0, rootId, 'Node 1') RETURNING Id INTO newId;
    INSERT INTO TreeNode VALUES(0, newId, 'Node 1.1');
    INSERT INTO TreeNode VALUES(0, newId, 'Node 1.2') RETURNING Id INTO newId;

    INSERT INTO TreeNode VALUES(0, newId, 'Node 1.2.1') RETURNING Id INTO newId;
    INSERT INTO TreeNode VALUES(0, newId, 'Node 1.2.1.1');

    INSERT INTO TreeNode VALUES(0, rootId, 'Node 2') RETURNING Id INTO newId;
    INSERT INTO TreeNode VALUES(0, newId, 'Node 2.1');
--    INSERT INTO TreeNode VALUES(0, newId, 'Node 2.2');
    INSERT INTO TreeNode VALUES(0, newId, 'Node 2.3');
    INSERT INTO TreeNode VALUES(0, newId, 'Node 2.4');

    INSERT INTO TreeNode VALUES(0, rootId, 'Node 3') RETURNING Id INTO newId;

    INSERT INTO TreeNode VALUES(0, rootId, 'Node 4') RETURNING Id INTO newId;
    INSERT INTO TreeNode VALUES(0, newId, 'Node 4.1');

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
SELECT LEVEL, Id, substr( LPAD (' ', 3 * (LEVEL - 1)) || Label, 0, 30) Node
FROM TreeNode
START WITH Parent = 0
CONNECT BY PRIOR Id = Parent;

-- Pseudo column: CONNECT_BY_ISLEAF
SELECT LEVEL, CONNECT_BY_ISLEAF Leaf, Id, substr( LPAD (' ', 3 * (LEVEL - 1)) || Label, 0, 30) Node
FROM TreeNode
START WITH Parent = 0
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
SELECT LEVEL, substr( SYS_CONNECT_BY_PATH(Id, '/'), 0, 60), 
	   Id,    substr( LPAD (' ', 3 * (LEVEL - 1)) || Label, 0, 30) Node
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


SELECT LEVEL, substr( SYS_CONNECT_BY_PATH(Id, '/'), 0, 30), 
	   PRIOR Id, Id,    substr( LPAD (' ', 3 * (LEVEL - 1)) || Label, 0, 30) Node
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

SELECT LEVEL, Id, substr( LPAD (' ', 3 * (LEVEL - 1)) || Label, 0, 30) Node
FROM TreeNode
START WITH Parent = 0
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
START WITH Parent = 0
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



-- Recursive With Clause
WITH Node (Id, label, parent, hierlevel, path) AS
( SELECT Id, Label, Parent, 1, label
  FROM   TreeNode
  WHERE  Parent = 0
  UNION ALL
  SELECT e.Id, e.Label, e.Parent, m.hierlevel + 1, m.path||'/'||e.label
  FROM TreeNode e
  JOIN Node m ON (m.Id = e.Parent)
)
SELECT hierlevel, substr( LPAD (' ', 4 * (hierlevel- 1)) || Label, 0, 30) "Node", 
				  substr( path, 0, 50) "Path"
FROM Node
ORDER BY path;
