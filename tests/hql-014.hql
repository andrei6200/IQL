SELECT P.firstName, P.lastName, 255 - Coll.red
FROM Persons AS P, PhotoCollection AS Coll
WHERE P.size < 5 * 1024
