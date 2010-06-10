SELECT P.firstName, P.lastName, JPEG(Coll)
FROM Persons AS P, PhotoCollection AS Coll
WHERE P.size < 5 * 1024
