-- SQL*Plus settings - restrict column sizes and enlarge line sizes and page sizes
SET LINESIZE 1000
SET PAGESIZE 300
COLUMN Label FORMAT A30
COLUMN Node FORMAT A30
COLUMN Path FORMAT A50

-- Remove existing schema objects
BEGIN DROP_TABLE_IF_EXISTS('GraphNode'); END;
BEGIN DROP_TABLE_IF_EXISTS('GraphEdge'); END;
BEGIN DROP_SEQUENCE_IF_EXISTS('GraphPk'); END;

-- Create a sequence for the primary key
CREATE SEQUENCE GraphPk
START WITH 1 
NOMAXVALUE
CACHE 100; 

-- Create the table which holds the graph nodes
CREATE TABLE GraphNode (
    Id NUMBER(10) PRIMARY KEY,
    Label VARCHAR2(100)
);

-- Create the table which holds the graph edges
CREATE TABLE GraphEdge (
    Id NUMBER(10) 	 PRIMARY KEY,
    Left NUMBER(10)  NOT NULL,
    Right NUMBER(10) NOT NULL,
    FOREIGN KEY (Left) REFERENCES GraphNode(Id),
    FOREIGN KEY (Right) REFERENCES GraphNode(Id)
);
CREATE INDEX GraphEdgeLeftPK ON GraphEdge(Left);
CREATE INDEX GraphEdgeRightPK ON GraphEdge(Right);


-- Create triggers for automatic primary key creation 
CREATE OR REPLACE TRIGGER GraphNodePkTrigger
BEFORE INSERT ON GraphNode
FOR EACH ROW
BEGIN
    SELECT GraphPk.NEXTVAL
    INTO   :NEW.ID
    FROM   DUAL;
END GraphNodePkTrigger;
/

CREATE OR REPLACE TRIGGER GraphEdgePkTrigger
BEFORE INSERT ON GraphEdge
FOR EACH ROW
BEGIN
    SELECT GraphPk.NEXTVAL
    INTO   :NEW.ID
    FROM   DUAL;
END GraphEdgePkTrigger;
/

-- Use an anonymous PL/SQL block to create sample data
DECLARE
    aNode NUMBER;
    bNode NUMBER;
    cNode NUMBER;
    dNode NUMBER;
    eNode NUMBER;
    fNode NUMBER;
    gNode NUMBER;

BEGIN 
    INSERT INTO GraphNode VALUES(null, 'Node A') RETURNING Id INTO aNode;
    INSERT INTO GraphNode VALUES(null, 'Node B') RETURNING Id INTO bNode;
    INSERT INTO GraphNode VALUES(null, 'Node C') RETURNING Id INTO cNode;
    INSERT INTO GraphNode VALUES(null, 'Node D') RETURNING Id INTO dNode;
    INSERT INTO GraphNode VALUES(null, 'Node E') RETURNING Id INTO eNode;
    INSERT INTO GraphNode VALUES(null, 'Node F') RETURNING Id INTO fNode;
    INSERT INTO GraphNode VALUES(null, 'Node G') RETURNING Id INTO gNode;

    INSERT INTO GraphEdge VALUES(null, aNode, dNode);
    INSERT INTO GraphEdge VALUES(null, aNode, eNode);
    INSERT INTO GraphEdge VALUES(null, bNode, dNode);
    INSERT INTO GraphEdge VALUES(null, bNode, eNode);
    INSERT INTO GraphEdge VALUES(null, cNode, dNode);

    INSERT INTO GraphEdge VALUES(null, dNode, fNode);
    INSERT INTO GraphEdge VALUES(null, eNode, fNode);
    INSERT INTO GraphEdge VALUES(null, eNode, gNode);

    COMMIT;
END;
/

SELECT * FROM GraphNode;
SELECT * FROM GraphEdge;

-- Hierarchical query starting at "Node B"
-- Selects all edges
SELECT LEVEL, Id, Left, Right
FROM GraphEdge
START WITH Left=2
CONNECT BY Left = PRIOR Right;

-- Hierarchical query starting at "Node B"
-- Selects all Right nodes. Note that "Node F" is in the result set twice,
-- once as a child of "Node D" and once as a child of "Node E"
SELECT Node.Id, substr( LPAD (' ', 3 * (LEVEL - 1)) || Node.Label, 0, 30) Node
FROM GraphEdge Edge
JOIN GraphNode Node ON Edge.Right = Node.id
START WITH Edge.Left=2
CONNECT BY Edge.Left = PRIOR Edge.Right;

        ID NODE
---------- ----------
         4 Node D
         6    Node F
         5 Node E
         6    Node F
         7    Node G

-- What happens with the above query if there is a cycle?
INSERT INTO GraphEdge VALUES(0, 6, 1);
COMMIT;

SELECT Node.Id, substr( LPAD (' ', 3 * (LEVEL - 1)) || Node.Label, 0, 30) Node
FROM GraphEdge Edge
JOIN GraphNode Node ON Edge.Right = Node.id
START WITH Edge.Left=2
CONNECT BY Edge.Left = PRIOR Edge.Right;

ERROR:
ORA-01436: CONNECT BY loop in user data
no rows selected


SELECT Node.Id, substr( LPAD (' ', 3 * (LEVEL - 1)) || Node.Label, 0, 30) Node, CONNECT_BY_ISCYCLE
FROM GraphEdge Edge
JOIN GraphNode Node ON Edge.Right = Node.id
START WITH Edge.Left=2
CONNECT BY NOCYCLE Edge.Left = PRIOR Edge.Right;



-- *************************
SELECT LEVEL, Edge.id, Edge.Left, Edge.Right
FROM GraphEdge Edge
START WITH Edge.Right=6
CONNECT BY PRIOR Edge.Left = Edge.Right;

SELECT Node.Id, substr( LPAD (' ', 3 * (LEVEL - 1)) || Node.Label, 0, 30) Node
FROM GraphEdge Edge
JOIN GraphNode Node ON Edge.Left = Node.id
START WITH Edge.Right=6
CONNECT BY PRIOR Edge.Left = Edge.Right;

-- Separately query start node
SELECT Node.Id, Node.Label Node
FROM GraphNode Node
WHERE Node.Id = 6;

-- Recursive Subquery Factoring

-- Simple query which starts from a specified node and traverses the graph 
-- from "left" to "right"
WITH Edge (Id, Left, Right) AS
(   -- Anchor Member - start node
    SELECT GraphEdge.Id, GraphEdge.Left, GraphEdge.Right
    FROM GraphEdge
    WHERE Left = 1	-- Start node
  UNION ALL
    -- Recursive Member - join between parent and child
    SELECT RightEdge.Id, RightEdge.Left, RightEdge.Right
    FROM GraphEdge RightEdge
    JOIN Edge LeftEdge ON (RightEdge.Left = LeftEdge.Right)
)
SEARCH DEPTH FIRST BY Id SET NodeSequence
SELECT * 
FROM Edge
ORDER BY NodeSequence;

-- "A row is considered to form a cycle if one of its ancestor rows has the 
--  same values for the cycle columns."
WITH Edge (Id, Left, Right, HLevel) AS
(   -- Anchor Member - start node
    SELECT GraphEdge.Id, GraphEdge.Left, GraphEdge.Right, 1
    FROM GraphEdge
    WHERE Left = 1	-- Start node
  UNION ALL
    -- Recursive Member - join between parent and child
    SELECT RightEdge.Id, RightEdge.Left, RightEdge.Right, LeftEdge.HLevel+1
    FROM GraphEdge RightEdge
    JOIN Edge LeftEdge ON (RightEdge.Left = LeftEdge.Right)
)
SEARCH DEPTH FIRST BY Id SET NodeSequence
CYCLE Right SET CycleFlag TO '*' DEFAULT ' '
SELECT e.Id, e.Left, e.Right, e.HLevel, LPAD (' ', 2 * (e.HLevel - 1)) || n.Label || CycleFlag Label, NodeSequence
FROM Edge e
JOIN GraphNode n ON (e.Right = n.Id) 
ORDER BY NodeSequence;

--
WITH Edge (Id, Left, Right, HLevel) AS
(   -- Anchor Member - start node
    SELECT null, null, Id, 1
    FROM GraphNode
    WHERE Id = 1	-- Start node
  UNION ALL
    -- Recursive Member - join between parent and child
    SELECT RightEdge.Id, RightEdge.Left, RightEdge.Right, LeftEdge.HLevel+1
    FROM GraphEdge RightEdge
    JOIN Edge LeftEdge ON (RightEdge.Left = LeftEdge.Right)
)
SEARCH DEPTH FIRST BY Id SET NodeSequence
CYCLE Right SET CycleFlag TO '*' DEFAULT ' '
SELECT e.Id, e.Left, e.Right, e.HLevel, LPAD (' ', 2 * (e.HLevel - 1)) || n.Label || CycleFlag Label, NodeSequence
FROM Edge e
JOIN GraphNode n ON (e.Right = n.Id) 
ORDER BY NodeSequence;

--
WITH Edge (Id, Left, Right, HLevel) AS
(   -- Anchor Member - start node
    SELECT null, Id, null, 1
    FROM GraphNode
    WHERE Id = 4	-- Start node
  UNION ALL
    -- Recursive Member - join between parent and child
    SELECT RightEdge.Id, RightEdge.Left, RightEdge.Right, LeftEdge.HLevel+1
    FROM GraphEdge RightEdge
    JOIN Edge LeftEdge ON (RightEdge.Right = LeftEdge.Left)
)
SEARCH DEPTH FIRST BY Id SET NodeSequence
CYCLE Left SET CycleFlag TO '*' DEFAULT ' '
SELECT e.Id, e.Left, e.Right, e.HLevel, LPAD (' ', 2 * (e.HLevel - 1)) || n.Label || CycleFlag Label, NodeSequence
FROM Edge e
JOIN GraphNode n ON (e.Left = n.Id) 
ORDER BY NodeSequence;
