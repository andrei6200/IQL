SELECT P.firstName, P.lastName, 255 - Coll.red
FROM Persons AS P, PhotoCollection AS Coll
LIMIT 50 OFFSET 10
