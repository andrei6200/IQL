SELECT P.dateTaken, P.firstName, P.lastName, P.age
FROM Persons AS P
WHERE P.size < 5 * 1024
