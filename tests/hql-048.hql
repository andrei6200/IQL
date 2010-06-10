SELECT P.firstName, P.lastName, JPEG(Coll)
FROM Persons AS P, PhotoCollection AS Coll
LIMIT 50 OFFSET 10
