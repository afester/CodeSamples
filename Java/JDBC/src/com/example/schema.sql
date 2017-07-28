
CREATE TABLE Performance(
    Col1 VARCHAR2(10),
    Col2 VARCHAR2(10),
    Col3 VARCHAR2(10),
    Col4 VARCHAR2(10),
    Col5 VARCHAR2(10),
    Col6 VARCHAR2(10),
    Col7 VARCHAR2(10),
    Col8 VARCHAR2(10),
    Col9 VARCHAR2(10),
    Col10 VARCHAR2(10)
);


BEGIN
    FOR cCount IN 1..4000 LOOP
        INSERT INTO Performance VALUES('ABCDEFGHIJ', 'KLMNOPQRST', 'UVWXYZabcd', 'fghijklmno', 'pqrstuvwxy', 
                                       'z123456789', '0ABCDEFGHI', 'KLMNOPQRST', 'UVWXYZabcd', 'efghijklmn');
    END LOOP;
END;
/

COMMIT;
