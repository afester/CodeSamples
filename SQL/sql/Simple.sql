-- Some very basic SQL queries

SELECT * FROM City;
SELECT * FROM Person;

-- Simple join - will not find persons with no city (assumed city is optional) 
-- and also no cities which are not assigned to any person
SELECT * 
FROM Person
JOIN City ON Person.City = City.Id;

-- Select all Persons with their cities, including Persons which do not have a city 
SELECT * 
FROM Person
LEFT OUTER JOIN City ON Person.City = City.Id;

-- Select all Persons with their assigned cities, and include Cities which are not assigned to any Person 
SELECT * 
FROM Person
RIGHT OUTER JOIN City ON Person.City = City.Id;

-- Select all Persons with their assigned cities, and include Cities which are 
-- not assigned to any Person AND Persons which do not have a City assigned 
SELECT * 
FROM Person
FULL JOIN City ON Person.City = City.Id;

-- Select the number of Persons in each City. Query is primarily on Person,
-- grouping the rows by the city name: 
SELECT City.Name, COUNT(*)
FROM Person
LEFT OUTER JOIN City ON Person.City = City.Id
GROUP BY City.Name
ORDER BY City.Name;

-- If only the city id is required, the join is not necessary: 
SELECT Person.City, COUNT(*)
FROM Person
GROUP BY Person.City;

-- Select all Persons with their cities, including Persons which do not have a 
-- city and print a useful value instead of null
SELECT Person.Firstname, Person.Name, Person.Email,
       CASE 
           WHEN Person.City IS NULL THEN 'N/A' 
           ELSE City.Name 
       END AS City
FROM Person
LEFT OUTER JOIN City ON Person.City = City.Id;
