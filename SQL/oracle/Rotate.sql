BEGIN DROP_TABLE_IF_EXISTS('Rotate'); END;

CREATE TABLE Rotate (
    Name VARCHAR2(100),
    Step VARCHAR2(100),
    Date DATE
);

INSERT INTO Rotate VALUES('Andreas', 'A', SYSDATE());
INSERT INTO Rotate VALUES('Andreas', 'B', SYSDATE());
INSERT INTO Rotate VALUES('Andreas', 'C', SYSDATE());
COMMIT;


-- *******************************************

drop table t_data;
create table t_data (
 C_ROW         NUMBER(4),
 C_COLUMN      NUMBER(4),
 C_VALUE       VARCHAR2(20)
);

insert into t_data values(0, 0, '1.1');
insert into t_data values(0, 1, '1.2');
insert into t_data values(0, 2, '1.3');
insert into t_data values(1, 0, '2.1');
insert into t_data values(1, 1, '2.2');
insert into t_data values(1, 2, '2.3');
insert into t_data values(2, 0, '3.1');
insert into t_data values(2, 1, '3.2');
insert into t_data values(2, 2, '3.3');
insert into t_data values(5, 3, '3.3');
commit;

select * from (
   select c_value, c_row, c_column
   from t_data t
)
pivot 
(
   MAX(c_value)
   for c_column in (0, 1, 2)
)
order by c_row;


select * from (
   select c_value, c_row, c_column
   from t_data t
)
pivot (
   MAX(c_value)
   for c_column in (0 as Col0, 1 as Col1, 2 as Col2, 3 as Col3)
)
order by c_row;


SELECT * FROM (
   SELECT ColumnsEO.C_ROW, 
          ColumnsEO.C_COLUMN, 
          ColumnsEO.C_VALUE
   FROM T_DATA ColumnsEO
)
PIVOT (
   MAX(c_value)
   for c_column in (0 as Col0, 1 as Col1, 2 as Col2, 3 as Col3)
)
order by c_row
