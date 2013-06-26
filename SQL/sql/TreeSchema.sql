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
    INSERT INTO TreeNode VALUES(0, newId, 'Node 2.2');
    INSERT INTO TreeNode VALUES(0, newId, 'Node 2.3');

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
SELECT LEVEL, Id, LPAD (' ', 4 * (LEVEL - 1)) || Label 
FROM TreeNode
START WITH Parent = 0
CONNECT BY PRIOR Id = Parent;


-- Path query - start with leaf and get all rows up to the root node
SELECT LEVEL, Id, LPAD (' ', 4 * (LEVEL - 1)) || Label 
FROM TreeNode
START WITH  Id=25
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
SELECT hierlevel, LPAD (' ', 4 * (hierlevel- 1)) || Label "Label", path "Path"
FROM Node
ORDER BY path;
