INSERT INTO Address VALUES(1, 'Andreas', 'Fester', 'andreas@example.com', 'MyCity', 'Streetname', '25', '12345', '555-123-4567', null);
INSERT INTO Address VALUES(2, 'Willi', 'Maier', 'willi@example.com', 'YourCity', 'Yourstreetname', '42', '42345', '555-321-4344', null);

INSERT INTO Task VALUES (1, None, 'Write Tutorials', 'Parent task for all tutorial related sub tasks');
  INSERT INTO Task VALUES (2, 1, 'Recursive subquery tutorial', 'Write a tutorial on recursive sub queries');
    INSERT INTO Task VALUES (3, 2, 'Initial draft', None);
    INSERT INTO Task VALUES (4, 2, 'Review', None);
  INSERT INTO Task VALUES (5, 1, 'ADF TreeTable tutorial', 'Write a tutorial about the ADF treetable');
    INSERT INTO Task VALUES (6, 5, 'Initial draft', None);
    INSERT INTO Task VALUES (7, 5, 'Review', None);
INSERT INTO Task VALUES (8, None, 'Implement Code', 'Parent task for all coding related tasks');
  INSERT INTO Task VALUES (9, 8, 'ADF TreeTable sample code', 'Implement a sample for the ADF treetable');
    INSERT INTO Task VALUES (10, 9, 'Coding', None);
    INSERT INTO Task VALUES (11, 9, 'Debugging', None);
  INSERT INTO Task VALUES (12, 8, 'ADF Custom EL resolver sample code', 'Implement a sample for implemeting a custom EL resovler');
    INSERT INTO Task VALUES (13, 12, 'Coding', None);
    INSERT INTO Task VALUES (14, 12, 'Debugging', None);

COMMIT;
EXIT;
