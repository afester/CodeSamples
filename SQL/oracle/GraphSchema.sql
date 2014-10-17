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
    Id NUMBER(10) NOT NULL,
    Label VARCHAR2(100)
);
CREATE UNIQUE INDEX GraphNodePK ON GraphNodeNode(Id);

-- Create the table which holds the graph edges
CREATE TABLE GraphEdge (
    Id NUMBER(10) 	 NOT NULL,
    Left NUMBER(10)  NOT NULL,
    Right NUMBER(10) NOT NULL
);
CREATE UNIQUE INDEX GraphEdgePK ON GraphEdge(Id);
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
    INSERT INTO GraphNode VALUES(0, 'Node A') RETURNING Id INTO aNode;
    INSERT INTO GraphNode VALUES(0, 'Node B') RETURNING Id INTO bNode;
    INSERT INTO GraphNode VALUES(0, 'Node C') RETURNING Id INTO cNode;
    INSERT INTO GraphNode VALUES(0, 'Node D') RETURNING Id INTO dNode;
    INSERT INTO GraphNode VALUES(0, 'Node E') RETURNING Id INTO eNode;
    INSERT INTO GraphNode VALUES(0, 'Node F') RETURNING Id INTO fNode;
    INSERT INTO GraphNode VALUES(0, 'Node G') RETURNING Id INTO gNode;

    INSERT INTO GraphEdge VALUES(0, aNode, dNode);
    INSERT INTO GraphEdge VALUES(0, aNode, eNode);
    INSERT INTO GraphEdge VALUES(0, bNode, dNode);
    INSERT INTO GraphEdge VALUES(0, bNode, eNode);
    INSERT INTO GraphEdge VALUES(0, cNode, dNode);

    INSERT INTO GraphEdge VALUES(0, dNode, fNode);
    INSERT INTO GraphEdge VALUES(0, eNode, fNode);
    INSERT INTO GraphEdge VALUES(0, eNode, gNode);

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
